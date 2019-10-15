/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * EWasm to binary encoder.
 */

#include <libyul/backends/wasm/BinaryTransform.h>

#include <libyul/Exceptions.h>
#include <libdevcore/CommonData.h>

#include <boost/range/adaptor/reversed.hpp>

using namespace std;
using namespace yul;
using namespace dev;
using namespace yul::wasm;

namespace
{

enum class Section: uint8_t
{
	TYPE = 0x01,
	IMPORT = 0x02,
	FUNCTION = 0x03,
	MEMORY = 0x05,
	EXPORT = 0x07,
	CODE = 0x0a
};

enum class ValueType: uint8_t
{
	Void = 0x40,
	Function = 0x60,
	I64 = 0x7e,
	I32 = 0x7f
};

enum class Export: uint8_t
{
	Function = 0x0,
	Memory = 0x2
};

enum class Opcode: uint8_t
{
	Unreachable = 0x00,// "unreachable", "")
	Nop = 0x01, //"nop", "")
	Block = 0x02,// "block", "")
	Loop = 0x03,// "loop", "")
	If = 0x04, //, "")
	Else = 0x05,// "else", "")
	Try = 0x06, //"try", "")
	Catch = 0x07,// "catch", "")
	Throw = 0x08,// "throw", "")
	Rethrow = 0x09,// "rethrow", "")
	BrOnExn = 0x0a,// "br_on_exn", "")
	End = 0x0b, //"end", "")
	Br = 0x0c, //, "")
	BrIf = 0x0d,// "br_if", "")
	BrTable = 0x0e,// "br_table", "")
	Return = 0x0f,// "return", "")
	Call = 0x10,// "call", "")
	CallIndirect = 0x11,// "call_indirect", "")
	ReturnCall = 0x12,// "return_call", "")
	ReturnCallIndirect = 0x13,// "return_call_indirect", "")
	Drop = 0x1a,// "drop", "")
	Select = 0x1b,// "select", "")
	LocalGet = 0x20,// "local.get", "")
	LocalSet = 0x21,// "local.set", "")
	LocalTee = 0x22,// "local.tee", "")
	GlobalGet = 0x23,// "global.get", "")
	GlobalSet = 0x24,// "global.set", "")
	I32Const = 0x41,// "i32.const", "")
	I64Const = 0x42,// "i64.const", "")
};

static std::map<string, uint8_t> const builtins = {
	{"i32.load", 0x28},
	{"i64.load", 0x29},
	{"i32.load8_s", 0x2c},
	{"i32.load8_u", 0x2d},
	{"i32.load16_s", 0x2e},
	{"i32.load16_u", 0x2f},
	{"i64.load8_s", 0x30},
	{"i64.load8_u", 0x31},
	{"i64.load16_s", 0x32},
	{"i64.load16_u", 0x33},
	{"i64.load32_s", 0x34},
	{"i64.load32_u", 0x35},
	{"i32.store", 0x36},
	{"i64.store", 0x37},
	{"i32.store8", 0x3a},
	{"i32.store16", 0x3b},
	{"i64.store8", 0x3c},
	{"i64.store16", 0x3d},
	{"i64.store32", 0x3e},
	{"memory.size", 0x3f},
	{"memory.grow", 0x40},
	{"i32.eqz", 0x45},
	{"i32.eq", 0x46},
	{"i32.ne", 0x47},
	{"i32.lt_s", 0x48},
	{"i32.lt_u", 0x49},
	{"i32.gt_s", 0x4a},
	{"i32.gt_u", 0x4b},
	{"i32.le_s", 0x4c},
	{"i32.le_u", 0x4d},
	{"i32.ge_s", 0x4e},
	{"i32.ge_u", 0x4f},
	{"i64.eqz", 0x50},
	{"i64.eq", 0x51},
	{"i64.ne", 0x52},
	{"i64.lt_s", 0x53},
	{"i64.lt_u", 0x54},
	{"i64.gt_s", 0x55},
	{"i64.gt_u", 0x56},
	{"i64.le_s", 0x57},
	{"i64.le_u", 0x58},
	{"i64.ge_s", 0x59},
	{"i64.ge_u", 0x5a},
	{"i32.clz", 0x67},
	{"i32.ctz", 0x68},
	{"i32.popcnt", 0x69},
	{"i32.add", 0x6a},
	{"i32.sub", 0x6b},
	{"i32.mul", 0x6c},
	{"i32.div_s", 0x6d},
	{"i32.div_u", 0x6e},
	{"i32.rem_s", 0x6f},
	{"i32.rem_u", 0x70},
	{"i32.and", 0x71},
	{"i32.or", 0x72},
	{"i32.xor", 0x73},
	{"i32.shl", 0x74},
	{"i32.shr_s", 0x75},
	{"i32.shr_u", 0x76},
	{"i32.rotl", 0x77},
	{"i32.rotr", 0x78},
	{"i64.clz", 0x79},
	{"i64.ctz", 0x7a},
	{"i64.popcnt", 0x7b},
	{"i64.add", 0x7c},
	{"i64.sub", 0x7d},
	{"i64.mul", 0x7e},
	{"i64.div_s", 0x7f},
	{"i64.div_u", 0x80},
	{"i64.rem_s", 0x81},
	{"i64.rem_u", 0x82},
	{"i64.and", 0x83},
	{"i64.or", 0x84},
	{"i64.xor", 0x85},
	{"i64.shl", 0x86},
	{"i64.shr_s", 0x87},
	{"i64.shr_u", 0x88},
	{"i64.rotl", 0x89},
	{"i64.rotr", 0x8a},
	{"i32.wrap_i64", 0xa7},
	{"i64.extend_i32_s", 0xac},
	{"i64.extend_i32_u", 0xad},
};

bytes opcode(Opcode _o)
{
	return bytes(1, uint8_t(_o));
}

bytes lebEncode(uint64_t _n)
{
	bytes encoded;
	while (_n > 0x7f)
	{
		encoded.emplace_back(uint8_t(0x80 | (_n & 0x7f)));
		_n >>= 7;
	}
	encoded.emplace_back(_n);
	return encoded;
}

}

bytes BinaryTransform::run(
	vector<GlobalVariableDeclaration> const& _globals,
	vector<FunctionImport> const& _imports,
	vector<FunctionDefinition> const& _functions
)
{
	m_globals.clear();
	for (size_t i = 0; i < _globals.size(); ++i)
		m_globals[_globals[i].variableName] = i;
	m_functions.clear();
	size_t funID = 0;
	for (FunctionImport const& fun: _imports)
		m_functions[fun.internalName] = funID++;
	for (FunctionDefinition const& fun: _functions)
		m_functions[fun.name] = funID++;

	bytes ret{0, 'a', 's', 'm'};
	ret += bytes{1, 0, 0, 0};
	ret += typeSection(_imports, _functions);
	ret += importSection(_imports);
	ret += functionSection(_functions);
	ret += memorySection();
	ret += exportSection();
	ret += codeSection(_functions);
	return ret;
}

bytes BinaryTransform::operator()(Literal const& _literal)
{
	return opcode(Opcode::I64Const) + lebEncode(_literal.value);
}

bytes BinaryTransform::operator()(StringLiteral const&)
{
	yulAssert(false, "String literals not yet implemented");
	// TODO is this used?
	return opcode(Opcode::I64Const) + lebEncode(0);
}

bytes BinaryTransform::operator()(LocalVariable const& _variable)
{
	return opcode(Opcode::LocalGet) + lebEncode(m_locals.at(_variable.name));
}

bytes BinaryTransform::operator()(GlobalVariable const& _variable)
{
	return opcode(Opcode::LocalGet) + lebEncode(m_globals.at(_variable.name));
}

bytes BinaryTransform::operator()(BuiltinCall const& _call)
{
	bytes args = visit(_call.arguments);

	if (
		_call.functionName.find(".load") != string::npos ||
		_call.functionName.find(".store") != string::npos
	)
		return std::move(args) + bytes{builtins.at(_call.functionName), 0, 0};
	else if (_call.functionName == "unreachable")
		return opcode(Opcode::Unreachable);
	else
		return std::move(args) + bytes(1, builtins.at(_call.functionName));
}

bytes BinaryTransform::operator()(FunctionCall const& _call)
{
	bytes result = visit(_call.arguments);
	result.push_back(uint8_t(Opcode::Call));
	result.push_back(m_functions.at(_call.functionName));
	return result;
}

bytes BinaryTransform::operator()(LocalAssignment const& _assignment)
{
	return
		boost::apply_visitor(*this, *_assignment.value) +
		opcode(Opcode::LocalSet) +
		lebEncode(m_locals.at(_assignment.variableName));
}

bytes BinaryTransform::operator()(GlobalAssignment const& _assignment)
{
	return
		boost::apply_visitor(*this, *_assignment.value) +
		opcode(Opcode::GlobalSet) +
		lebEncode(m_globals.at(_assignment.variableName));
}

bytes BinaryTransform::operator()(If const& _if)
{
	bytes result =
		boost::apply_visitor(*this, *_if.condition) +
		opcode(Opcode::If) +
		bytes(1, uint8_t(ValueType::Void));

	m_labels.push({});

	result += visit(_if.statements);
	if (_if.elseStatements)
		result += opcode(Opcode::Else) + visit(*_if.elseStatements);

	m_labels.pop();

	result += opcode(Opcode::End);
	return result;
}

bytes BinaryTransform::operator()(Loop const& _loop)
{
	bytes result =
		opcode(Opcode::Loop) +
		bytes(1, uint8_t(ValueType::Void));

	m_labels.push(_loop.labelName);
	result += visit(_loop.statements);
	m_labels.pop();

	result += opcode(Opcode::End);
	return result;
}

bytes BinaryTransform::operator()(Break const& _break)
{
	// TOOD the index is just the nesting depth.
	return {};
}

bytes BinaryTransform::operator()(Continue const&)
{
	yulAssert(false, "Continue does not exist in wasm.");
	// TOOD the index is just the nesting depth.
	return {};
}

bytes BinaryTransform::operator()(Block const& _block)
{
	return
		opcode(Opcode::Block) +
		bytes(1, uint8_t(ValueType::Void)) +
		visit(_block.statements) +
		opcode(Opcode::End);
}

bytes BinaryTransform::operator()(FunctionDefinition const& _function)
{
	bytes ret;

	// TODO this is a kind of run-length-encoding of local types. Has to be adapted once
	// we have locals of different types.
	ret += lebEncode(1); // number of locals groups
	ret += lebEncode(_function.locals.size());
	ret.push_back(uint8_t(ValueType::I64));

	m_locals.clear();
	size_t varIdx = 0;
	for (size_t i = 0; i < _function.parameterNames.size(); ++i)
		m_locals[_function.parameterNames[i]] = varIdx++;
	for (size_t i = 0; i < _function.locals.size(); ++i)
		m_locals[_function.locals[i].variableName] = varIdx++;

	ret += visit(_function.body);
	ret.push_back(uint8_t(Opcode::End));

	return prefixSize(std::move(ret));
}

BinaryTransform::Type BinaryTransform::typeOf(FunctionImport const& _import)
{
	return {
		encodeTypes(_import.paramTypes),
		encodeTypes(_import.returnType ? vector<string>(1, *_import.returnType) : vector<string>())
	};
}

BinaryTransform::Type BinaryTransform::typeOf(FunctionDefinition const& _funDef)
{

	return {
		encodeTypes(vector<string>(_funDef.parameterNames.size(), "i64")),
		encodeTypes(vector<string>(_funDef.returns ? 1 : 0, "i64"))
	};
}

uint8_t BinaryTransform::encodeType(string const& _typeName)
{
	if (_typeName == "i32")
		return uint8_t(ValueType::I32);
	else if (_typeName == "i64")
		return uint8_t(ValueType::I64);
//	else
	//		yulAssert(false, "");
	return 0;
}

vector<uint8_t> BinaryTransform::encodeTypes(vector<string> const& _typeNames)
{
	vector<uint8_t> result;
	for (auto const& t: _typeNames)
		result.emplace_back(encodeType(t));
	return result;
}

bytes BinaryTransform::typeSection(
	vector<FunctionImport> const& _imports,
	vector<FunctionDefinition> const& _functions
)
{
	map<Type, vector<string>> types;
	for (auto const& import: _imports)
		types[typeOf(import)].emplace_back(import.internalName);
	for (auto const& fun: _functions)
		types[typeOf(fun)].emplace_back(fun.name);

	bytes result;
	size_t index = 0;
	for (auto const& [type, funNames]: types)
	{
		for (string const& name: funNames)
			m_functionTypes[name] = index;
		result.emplace_back(uint8_t(ValueType::Function));
		result += lebEncode(type.first.size()) + type.first;
		result += lebEncode(type.second.size()) + type.second;

		index++;
	}

	return bytes(1, uint8_t(Section::TYPE)) + prefixSize(lebEncode(index) + std::move(result));
}

bytes BinaryTransform::importSection(
	vector<FunctionImport> const& _imports
)
{
	bytes result = lebEncode(_imports.size());
	for (FunctionImport const& import: _imports)
	{
		uint8_t importKind = 0; // function
		result +=
			encode(import.module) +
			encode(import.externalName) +
			bytes(1, importKind) +
			lebEncode(m_functionTypes[import.internalName]);
	}
	return bytes(1, uint8_t(Section::IMPORT)) + prefixSize(std::move(result));
}

bytes BinaryTransform::functionSection(vector<FunctionDefinition> const& _functions)
{
	bytes result = lebEncode(_functions.size());
	for (auto const& fun: _functions)
		result += lebEncode(m_functionTypes.at(fun.name));
	return bytes(1, uint8_t(Section::FUNCTION)) + prefixSize(std::move(result));
}

bytes BinaryTransform::memorySection()
{
	bytes result = lebEncode(1);
	result.push_back(0); // flags
	result.push_back(1); // initial
	return bytes(1, uint8_t(Section::MEMORY)) + prefixSize(std::move(result));
}

bytes BinaryTransform::exportSection()
{
	bytes result = lebEncode(2);
	result += encode("memory") + bytes(1, uint8_t(Export::Memory)) + lebEncode(0);
	result += encode("main") + bytes(1, uint8_t(Export::Function)) + lebEncode(m_functions.at("main"));
	return bytes(1, uint8_t(Section::EXPORT)) + prefixSize(std::move(result));
}

bytes BinaryTransform::codeSection(vector<wasm::FunctionDefinition> const& _functions)
{
	bytes result = lebEncode(_functions.size());
	for (FunctionDefinition const& fun: _functions)
		result += (*this)(fun);
	return bytes(1, uint8_t(Section::CODE)) + prefixSize(std::move(result));
}

bytes BinaryTransform::visit(vector<Expression> const& _expressions)
{
	bytes result;
	for (auto const& expr: _expressions)
		result += boost::apply_visitor(*this, expr);
	return result;
}

bytes BinaryTransform::visitReversed(vector<Expression> const& _expressions)
{
	bytes result;
	for (auto const& expr: _expressions | boost::adaptors::reversed)
		result += boost::apply_visitor(*this, expr);
	return result;
}

bytes BinaryTransform::prefixSize(bytes _data)
{
	size_t size = _data.size();
	return lebEncode(size) + std::move(_data);
}

bytes BinaryTransform::encode(std::string const& _name)
{
	return lebEncode(_name.size()) + asBytes(_name);
}
