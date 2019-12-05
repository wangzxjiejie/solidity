##########################
Abstract syntax tree (AST)
##########################

This guide documents the Solidity abstract syntax tree (AST), representing the abstract structure of code written in Solidity. It describes each node in the tree, parameters, and potential direct children.

The ``--ast-compact-json`` option generates an AST of all source files in a compact JSON format, useful for generating symbol tables for compilers and analysis tools.

The JSON output typically consists of an array of nodes (with an associated ``nodeType``) with key/value pair fields. There are a lot of these, and depending on the contract structure, can have varying nested structures. You can see examples of the input contracts and their JSON AST output `in the Solidity test suite <https://github.com/ethereum/solidity/tree/develop/test/libsolidity/ASTJSON>`_, but below is an example with explanation.

Input
-----

::
    contract C { function f(function() external payable returns (uint) x) returns (function() external view returns (uint)) {} }

Output
------

.. code-block:: none

    {
        "absolutePath": "a",
        "exportedSymbols": {
            "C": [
                17
            ]
        },
        "id": 18,
        "nodeType": "SourceUnit",
        "nodes": [
            {
                "baseContracts": [],
                "contractDependencies": [],
                "contractKind": "contract",
                "documentation": null,
                "fullyImplemented": true,
                "id": 17,
                "linearizedBaseContracts": [
                    17
                ],
                "name": "C",
                "nodeType": "ContractDefinition",
                "nodes": [
                    {
                        "body": {
                            "id": 15,
                            "nodeType": "Block",
                            "src": "120:2:1",
                            "statements": []
                        },
                        "documentation": null,
                        "id": 16,
                        "implemented": true,
                        "kind": "function",
                        "modifiers": [],
                        "name": "f",
                        "nodeType": "FunctionDefinition",
                        "parameters": {
                            "id": 7,
                            "nodeType": "ParameterList",
                            "parameters": [
                                {
                                    "constant": false,
                                    "id": 6,
                                    "name": "x",
                                    "nodeType": "VariableDeclaration",
                                    "scope": 16,
                                    "src": "24:44:1",
                                    "stateVariable": false,
                                    "storageLocation": "default",
                                    "typeDescriptions": {
                                        "typeIdentifier": "t_function_external_payable$__$returns$_t_uint256_$",
                                        "typeString": "function () payable external returns (uint256)"
                                    },
                                    "typeName": {
                                        "id": 5,
                                        "nodeType": "FunctionTypeName",
                                        "parameterTypes": {
                                            "id": 1,
                                            "nodeType": "ParameterList",
                                            "parameters": [],
                                            "src": "32:2:1"
                                        },
                                        "returnParameterTypes": {
                                            "id": 4,
                                            "nodeType": "ParameterList",
                                            "parameters": [
                                                {
                                                    "constant": false,
                                                    "id": 3,
                                                    "name": "",
                                                    "nodeType": "VariableDeclaration",
                                                    "scope": 5,
                                                    "src": "61:4:1",
                                                    "stateVariable": false,
                                                    "storageLocation": "default",
                                                    "typeDescriptions": {
                                                        "typeIdentifier": "t_uint256",
                                                        "typeString": "uint256"
                                                    },
                                                    "typeName": {
                                                        "id": 2,
                                                        "name": "uint",
                                                        "nodeType": "ElementaryTypeName",
                                                        "src": "61:4:1",
                                                        "typeDescriptions": {
                                                            "typeIdentifier": "t_uint256",
                                                            "typeString": "uint256"
                                                        }
                                                    },
                                                    "value": null,
                                                    "visibility": "internal"
                                                }
                                            ],
                                            "src": "60:6:1"
                                        },
                                        "src": "24:44:1",
                                        "stateMutability": "payable",
                                        "typeDescriptions": {
                                            "typeIdentifier": "t_function_external_payable$__$returns$_t_uint256_$",
                                            "typeString": "function () payable external returns (uint256)"
                                        },
                                        "visibility": "external"
                                    },
                                    "value": null,
                                    "visibility": "internal"
                                }
                            ],
                            "src": "23:46:1"
                        },
                        "returnParameters": {
                            "id": 14,
                            "nodeType": "ParameterList",
                            "parameters": [
                                {
                                    "constant": false,
                                    "id": 13,
                                    "name": "",
                                    "nodeType": "VariableDeclaration",
                                    "scope": 16,
                                    "src": "79:40:1",
                                    "stateVariable": false,
                                    "storageLocation": "default",
                                    "typeDescriptions": {
                                        "typeIdentifier": "t_function_external_view$__$returns$_t_uint256_$",
                                        "typeString": "function () view external returns (uint256)"
                                    },
                                    "typeName": {
                                        "id": 12,
                                        "nodeType": "FunctionTypeName",
                                        "parameterTypes": {
                                            "id": 8,
                                            "nodeType": "ParameterList",
                                            "parameters": [],
                                            "src": "87:2:1"
                                        },
                                        "returnParameterTypes": {
                                            "id": 11,
                                            "nodeType": "ParameterList",
                                            "parameters": [
                                                {
                                                    "constant": false,
                                                    "id": 10,
                                                    "name": "",
                                                    "nodeType": "VariableDeclaration",
                                                    "scope": 12,
                                                    "src": "113:4:1",
                                                    "stateVariable": false,
                                                    "storageLocation": "default",
                                                    "typeDescriptions": {
                                                        "typeIdentifier": "t_uint256",
                                                        "typeString": "uint256"
                                                    },
                                                    "typeName": {
                                                        "id": 9,
                                                        "name": "uint",
                                                        "nodeType": "ElementaryTypeName",
                                                        "src": "113:4:1",
                                                        "typeDescriptions": {
                                                            "typeIdentifier": "t_uint256",
                                                            "typeString": "uint256"
                                                        }
                                                    },
                                                    "value": null,
                                                    "visibility": "internal"
                                                }
                                            ],
                                            "src": "112:6:1"
                                        },
                                        "src": "79:40:1",
                                        "stateMutability": "view",
                                        "typeDescriptions": {
                                            "typeIdentifier": "t_function_external_view$__$returns$_t_uint256_$",
                                            "typeString": "function () view external returns (uint256)"
                                        },
                                        "visibility": "external"
                                    },
                                    "value": null,
                                    "visibility": "internal"
                                }
                            ],
                            "src": "78:41:1"
                        },
                        "scope": 17,
                        "src": "13:109:1",
                        "stateMutability": "nonpayable",
                        "superFunction": null,
                        "visibility": "public"
                    }
                ],
                "scope": 18,
                "src": "0:124:1"
            }
        ],
        "src": "0:125:1"
    }

The ``nodeType`` from the above example are the following:

- ``SourceUnit``
- ``ContractDefinition``
- ``Block``
- ``FunctionDefinition``
- ``ParameterList``
- ``VariableDeclaration``
- ``FunctionTypeName``
- ``ElementaryTypeName``

The fields from the above example are the following:

- ``absolutePath``: The absolute path of the source unit to import
- ``exportedSymbols``: The exported symbols (all global symbols)
- ``id``: TBD
- ``nodeType``: The type of node/declaration
- ``baseContracts``: Contracts this contract inherits from
- ``contractDependencies``: Libraries this contract uses
- ``contractKind``: Is this an interface, contract, or library
- ``documentation``: Is function documented
- ``fullyImplemented``: ``false`` if this is an abstract contract
- ``linearizedBaseContracts``: 	All direct and indirect base contracts from derived to base, including this contract
- ``name``: User-defined name of the contract, function, library or variable
- ``body``: Body of the node that likely contains further nodes
- ``src``: TBD
- ``statements``: Array of zero or more statements
- ``implemented``: Is the function fully implemented
- ``kind``: Human readable node type
- ``modifiers``: Array of function modifier objects
- ``parameters``: Array of objects representing function parameters
- ``constant``: Is parameter declared as a constant
- ``scope``: Stores a reference to the current contract. Needed because types of base contracts change depending on the context
- ``stateVariable``: Is the variable a state variable
- ``storageLocation``: Memory storage location for variable
- ``typeDescriptions``: Object containing variable type details
    - ``typeIdentifier``: TBD
    - ``typeString``: String representing the variable type
- ``typeName``: TBD
- ``returnParameterTypes``: Object containing details on function return parameters
- ``value``: TBD
- ``visibility``: Visibility of the function or variable
- ``stateMutability``: The mutability of the function or variable