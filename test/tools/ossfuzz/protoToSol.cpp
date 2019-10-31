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

#include <sstream>
#include "protoToSol.h"

#include <libdevcore/Whiskers.h>

using namespace dev;
using namespace dev::test::sol_fuzzer;
using namespace std;

string ProtoConverter::protoToSolidity(Program const& _p)
{
	return visit(_p);
}

string ProtoConverter::visit(Program const& _p)
{
	ostringstream program;
	program << Whiskers(R"(pragma solidity >=0.0;

	<programType> <programName>
	{
<contractBody>
	})")
	("programType", _p.type() == Program_Type::Program_Type_CONTRACT ? "contract" : "library")
	("programName", _p.type() == Program_Type::Program_Type_CONTRACT ? "C" : "L")
	("contractBody", "")
	.render();
	return program.str();
}

string ProtoConverter::visit(Block const&)
{
	return "";
}

string ProtoConverter::visit(Statement const&)
{
	return "";
}

string ProtoConverter::visit(VarDecl const&)
{
	return "";
}

string ProtoConverter::visit(IfStmt const&)
{
	return "";
}

string ProtoConverter::visit(ForStmt const&)
{
	return "";
}

string ProtoConverter::visit(SwitchStmt const&)
{
	return "";
}

string ProtoConverter::visit(BreakStmt const&)
{
	return "";
}

string ProtoConverter::visit(ContinueStmt const&)
{
	return "";
}

string ProtoConverter::visit(ReturnStmt const&)
{
	return "";
}

string ProtoConverter::visit(DoStmt const&)
{
	return "";
}

string ProtoConverter::visit(WhileStmt const&)
{
	return "";
}