##########################
Abstract syntax tree (AST)
##########################

This guide documents the Solidity abstract syntax tree (AST), representing the abstract structure of code written in Solidity. It describes each node in the tree, parameters, and potential direct children.

Common fields
-------------

All nodes share the same fields:

- ``Source``: Source code line(s) of the AST node.

Nodes
-----

PragmaDirective
===============

Pragma directive for the contract definition, only version requirements in the form `pragma solidity "^0.4.0";` are supported.

ContractDefinition "ContractName"
=================================

The definition of a contract or library, followed by the name of the contract. This is the only AST node where child nodes are not visited in document order. It first visits all struct declarations, then all variable declarations and finally all function declarations.

Child nodes
~~~~~~~~~~~

All other nodes.

CallableDeclaration
===================

Base class for all nodes that define function-like objects, i.e., ``FunctionDefinition``, ``EventDefinition`` and ``ModifierDefinition``.

Child nodes:

- ``ParameterList``
- ``Block``

FunctionDefinition "FunctionName"
=================================

Definition of a function and its name. Uses the same parameters and child nodes as ``CallableDeclaration``.

EventDefinition "EventName"
===========================

Definition of an event and its name. Uses the same parameters and child nodes as ``CallableDeclaration``.

ModifierDefinition "modifierName"
=================================

Definition of a function modifier and its name. Uses the same parameters and child nodes as ``CallableDeclaration``.

ParameterList
=============

A list of function parameters and return variables. Does not include mappings.
.. TODO: And structs?

Child nodes:

- ``VariableDeclaration`` "VariableName" (multiple)

StructDefinition "VariableName"
===============================

Definition of a ``struct`` variable and its name.

Child nodes:

- ``VariableDeclaration`` "VariableName" (multiple)

VariableDeclaration "VariableName"
==================================

Declaration of a variable and its name.

Fields:

``Type``: Variable type including any size

Child nodes:

- ``Mapping`` "MappingName"
- ``ElementaryTypeName`` "BaseType"
- ``ArrayTypeName``

Mapping "MappingName"
=====================

A mapping type and its name.

Child nodes:

- ``ElementaryTypeName`` "BaseType"
- ``UserDefinedTypeName`` "VariableName"

ElementaryTypeName "BaseType"
=============================

Any pre-defined type name represented by a single keyword (and state mutability for address types). Excludes mappings, contracts, functions, etc.

ArrayTypeName
=============

An array type that can be empty or contain an expression.

Child nodes:

- ``UserDefinedTypeName`` "VariableName"

UserDefinedTypeName "VariableName"
==================================

A user-defined type (i.e., a struct, contract, etc.).

Block
=====

Brace-enclosed block containing zero or more statements.

Child nodes:

- ``ExpressionStatement`` (multiple)
- ``ForStatement`` (multiple)
- ``EmitStatement`` (multiple)

ExpressionStatement
===================

A statement that contains one expression (i.e., an assignment, function call, etc.).

Child nodes:

- ``FunctionCall`` (multiple)

FunctionCall
============

.. TODO: I don't really understand this

A function call, type cast or struct construction.

Child nodes:

- Identifier
- BinaryOperation
- Literal

Identifier "identifier"
=======================

An identifier, i.e., a reference to a declaration by name, such as a variable or function.

.. TODO: Are fields just nodes?

Fields:

- Type

BinaryOperation
===============

Operation involving a binary operator. For example: ``1 + 2``, ``true && false`` or ``1 <= 4``.

Fields:

- Type

Child nodes:

- Identifier
- MemberAccess

Literal something?
==================

A literal string or number.

Fields:

- Type

MemberAccess something?
=======================

Access to a member of an object. For example: `x.name`.

Fields:

- Type

Child nodes:

- IndexAccess

IndexAccess something?
======================

Index access to an array or mapping. For example: `a[2]`.

Fields:

- Type

Child nodes:

- Identifier

---

VariableDeclarationStatement
============================

Definition of one or more variables as a statement inside a function. If multiple variables are declared, a value has to be assigned directly. If only a single variable is declared, the value can be missing.

Examples:
uint[] memory a; uint a = 2;
(uint a, bytes32 b, ) = f(); (, uint a, , StructName storage x) = g();






Return
======
Break
=====
Continue
========





UnaryOperation
==============
Operation involving a unary operator, pre- or postfix.
Examples: ++i, delete x or !true





ForStatement
============
For loop statement

WhileStatement
==============
IfStatement
===========
If-statement with an optional "else" part. Note that "else if" is modeled by having a new if-statement as the false (else) body.
EmitStatement
=============