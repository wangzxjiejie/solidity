##########################
Abstract syntax tree (AST)
##########################

This guide documents the Solidity abstract syntax tree (AST), representing the abstract structure of code written in Solidity. It describes each node in the tree, parameters, and potential direct children.

.. TODO: Abstract classes from h file? Are they appended?

Common fields
-------------

All nodes share the same fields:

- ``Source``: Source code line(s) of the AST node.

Nodes
-----

PragmaDirective
===============

Pragma directive for the contract definition, only version requirements in the form `pragma solidity "^0.4.0";` are supported.

ImportDirective "file"
======================

Import directive for referencing other files / source objects.

ContractDefinition "ContractName"
=================================

The definition of a contract or library, followed by the name of the contract. This is the only AST node where child nodes are not visited in document order. It first visits all struct declarations, then all variable declarations and finally all function declarations.

Child nodes
~~~~~~~~~~~

All other nodes.

UsingForDirective
=================

``using LibraryName for variable`` directive that attaches all functions from the library LibraryName the variable if the first parameter matches the type. ``using LibraryName for *`` attaches the function to any matching type.

Child nodes:

- ``UserDefinedTypeName``

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

ModifierInvocation
==================

.. TODO

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

EnumDefinition "VariableName"
===============================

Definition of a ``enum`` variable and its name.

Child nodes:

- ``EnumValue`` "VariableName" (multiple)

EnumValue
=========

Declaration of an Enum Value.

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

MappingTypeName
===============

.. TODO: Actually exist?

FunctionTypeName
================

..  A literal function type. Its source form is "function (paramType1, paramType2) internal / external returns (retType1, retType2)"

.. TODO

UserDefinedTypeName "VariableName"
==================================

A user-defined type (i.e., a struct, contract, etc.).

Block
=====

Brace-enclosed block containing zero or more statements.

Child nodes:

- ``ExpressionStatement``
- ``ForStatement``
- ``EmitStatement``
- ``VariableDeclarationStatement``
- ``WhileStatement``
- ``Return``

ExpressionStatement
===================

A statement that contains one expression (i.e., an assignment, function call, etc.).

Child nodes:

- ``FunctionCall`` (multiple)
- ``UnaryOperation``
- Assignment

Assignment
==========

Assignment, can also be a compound assignment, e.g., ``(a = 7 + 8)`` or ``(a *= 2)``

Fields:

- Type

Child nodes:

- Identifier
- MemberAccess

EmitStatement
=============

The emit statement is used to emit events.

- Child nodes:

- ``FunctionCall``

FunctionCall
============

.. TODO: I don't really understand this

A function call, type cast or struct construction.

Child nodes:

- Identifier
- BinaryOperation
- Literal
- UnaryOperation
- ForStatement

.. TODO: maybe switch items after node type under heading in some way?

Return "identifier"
===================

Return statement to return a variable value.

Child nodes:

- Identifier

Break
=====

A break statement that terminates the current loop, switch, or label statement and transfers program control to the statement following the terminated statement.

Continue
========

A continue statement that terminates execution of the statement in the current iteration of the current loop, and continues execution of the loop with the next iteration.

Identifier "identifier"
=======================

An identifier, i.e., a reference to a declaration by name, such as a variable or function.

.. TODO: Are fields just nodes?

Fields:

- Type


UnaryOperation "(pre or postfix) operation"
===========================================

Operation involving a unary operator, pre- or postfix. For example: ``++i``, ``delete x`` or ``!true``

Fields:

- Type

Child nodes:

- Identifier

ForStatement
============

For loop statement.

Child nodes:

- VariableDeclarationStatement
- BinaryOperation
- ExpressionStatement
- Block

WhileStatement
==============

While loop statement.

- BinaryOperation
- Block

IfStatement
===========
If statement with an optional "else" part. Note that "else if" is modeled by having a new if statement as the false (else) body.

Child nodes:

-  ``BinaryOperation``
- ``Block``

VariableDeclarationStatement
============================

Definition of one or more variables as a statement inside a function. If multiple variables are declared, a value has to be assigned directly. If only a single variable is declared, the value can be missing.

Examples:

- ``uint[] memory a; uint a = 2;``
- ``(uint a, bytes32 b, ) = f();``
- ``(, uint a, , StructName storage x) = g();``

Child nodes:

- VariableDeclaration
- Literal

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

Access to a member of an object. For example: ``x.name``.

Fields:

- Type

Child nodes:

- IndexAccess

IndexAccess something?
======================

Index access to an array or mapping. For example: ``a[2]``.

Fields:

- Type

Child nodes:

- Identifier

NewExpression
=============

Expression that creates a new contract or memory-array, e.g., the "new SomeContract" part in "new SomeContract(1, 2)".

Fields:

- Type

Child nodes:

- UserDefinedTypeName

TupleExpression
===============

Tuple, parenthesized expression, or bracketed expression, e.g., (1, 2), (x,), (x), (), [1, 2]. Individual components might be empty shared pointers (as in the second example).

The respective types in lvalue context are: 2-tuple, 2-tuple (with wildcard), type of x, 0-tuple. Not in lvalue context: 2-tuple, _1_-tuple, type of x, 0-tuple.

Fields:

- Type

Child nodes:

- Literal

PrimaryExpression
=================

.. TODO

ElementaryTypeNameExpression
============================

.. TODO: Needed?

InlineAssembly
==============

Inline assembly.

.. TODO

Throw
=====

.. The Throw statement to throw that triggers a solidity exception(jump to ErrorTag)

.. TODO