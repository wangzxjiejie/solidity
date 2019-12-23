#pragma once

#include <libsolidity/interface/CompilerStack.h>

#include <libyul/AssemblyStack.h>

#include <liblangutil/Exceptions.h>
#include <liblangutil/SourceReferenceFormatter.h>

#include <libdevcore/Keccak256.h>

namespace dev
{
namespace test
{
namespace abiv2fuzzer
{
class SolidityCompilationFramework
{
public:
	explicit SolidityCompilationFramework(langutil::EVMVersion _evmVersion = {});

	Json::Value getMethodIdentifiers()
	{
		return m_compiler.methodIdentifiers(m_compiler.lastContractName());
	}
	dev::bytes compileContract(
		std::string const& _sourceCode,
		std::string const& _contractName = {}
	);
protected:
	solidity::frontend::CompilerStack m_compiler;
	langutil::EVMVersion m_evmVersion;
	solidity::frontend::OptimiserSettings m_optimiserSettings = solidity::frontend::OptimiserSettings::none();
};
}
}
}
