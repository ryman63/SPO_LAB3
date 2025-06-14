/** \file
 *  This C header file was generated by $ANTLR version 3.4
 *
 *     -  From the grammar source file : custom_lang.g3
 *     -                            On : 2025-06-08 20:21:17
 *     -                for the parser : custom_langParserParser
 *
 * Editing it, at least manually, is not wise.
 *
 * C language generator and runtime by Jim Idle, jimi|hereisanat|idle|dotgoeshere|ws.
 *
 *
 * The parser 
custom_langParser

has the callable functions (rules) shown below,
 * which will invoke the code for the associated rule in the source grammar
 * assuming that the input stream is pointing to a token/text stream that could begin
 * this rule.
 *
 * For instance if you call the first (topmost) rule in a parser grammar, you will
 * get the results of a full parse, but calling a rule half way through the grammar will
 * allow you to pass part of a full token stream to the parser, such as for syntax checking
 * in editors and so on.
 *
 * The parser entry points are called indirectly (by function pointer to function) via
 * a parser context typedef pcustom_langParser, which is returned from a call to custom_langParserNew().
 *
 * The methods in pcustom_langParser are  as follows:
 *
 *  - 
 custom_langParser_source_return
      pcustom_langParser->source(pcustom_langParser)
 *  - 
 custom_langParser_sourceItem_return
      pcustom_langParser->sourceItem(pcustom_langParser)
 *  - 
 custom_langParser_type_return
      pcustom_langParser->type(pcustom_langParser)
 *  - 
 custom_langParser_basicType_return
      pcustom_langParser->basicType(pcustom_langParser)
 *  - 
 custom_langParser_typeRef_return
      pcustom_langParser->typeRef(pcustom_langParser)
 *  - 
 custom_langParser_arrayType_return
      pcustom_langParser->arrayType(pcustom_langParser)
 *  - 
 custom_langParser_funcSignature_return
      pcustom_langParser->funcSignature(pcustom_langParser)
 *  - 
 custom_langParser_argList_return
      pcustom_langParser->argList(pcustom_langParser)
 *  - 
 custom_langParser_arg_return
      pcustom_langParser->arg(pcustom_langParser)
 *  - 
 custom_langParser_funcDef_return
      pcustom_langParser->funcDef(pcustom_langParser)
 *  - 
 custom_langParser_basicStatement_return
      pcustom_langParser->basicStatement(pcustom_langParser)
 *  - 
 custom_langParser_statement_return
      pcustom_langParser->statement(pcustom_langParser)
 *  - 
 custom_langParser_simpleStatement_return
      pcustom_langParser->simpleStatement(pcustom_langParser)
 *  - 
 custom_langParser_repeatStatement_return
      pcustom_langParser->repeatStatement(pcustom_langParser)
 *  - 
 custom_langParser_ifStatement_return
      pcustom_langParser->ifStatement(pcustom_langParser)
 *  - 
 custom_langParser_loopStatement_return
      pcustom_langParser->loopStatement(pcustom_langParser)
 *  - 
 custom_langParser_breakStatement_return
      pcustom_langParser->breakStatement(pcustom_langParser)
 *  - 
 custom_langParser_expressionStatement_return
      pcustom_langParser->expressionStatement(pcustom_langParser)
 *  - 
 custom_langParser_blockStatement_return
      pcustom_langParser->blockStatement(pcustom_langParser)
 *  - 
 custom_langParser_argument_expression_list_return
      pcustom_langParser->argument_expression_list(pcustom_langParser)
 *  - 
 custom_langParser_additive_expression_return
      pcustom_langParser->additive_expression(pcustom_langParser)
 *  - 
 custom_langParser_multiplicative_expression_return
      pcustom_langParser->multiplicative_expression(pcustom_langParser)
 *  - 
 custom_langParser_unary_expression_return
      pcustom_langParser->unary_expression(pcustom_langParser)
 *  - 
 custom_langParser_postfix_expression_return
      pcustom_langParser->postfix_expression(pcustom_langParser)
 *  - 
 custom_langParser_array_expression_return
      pcustom_langParser->array_expression(pcustom_langParser)
 *  - 
 custom_langParser_primary_expression_list_return
      pcustom_langParser->primary_expression_list(pcustom_langParser)
 *  - 
 custom_langParser_unary_operator_return
      pcustom_langParser->unary_operator(pcustom_langParser)
 *  - 
 custom_langParser_primary_expression_return
      pcustom_langParser->primary_expression(pcustom_langParser)
 *  - 
 custom_langParser_expression_return
      pcustom_langParser->expression(pcustom_langParser)
 *  - 
 custom_langParser_constant_expression_return
      pcustom_langParser->constant_expression(pcustom_langParser)
 *  - 
 custom_langParser_assignment_expression_return
      pcustom_langParser->assignment_expression(pcustom_langParser)
 *  - 
 custom_langParser_lvalue_return
      pcustom_langParser->lvalue(pcustom_langParser)
 *  - 
 custom_langParser_assignment_operator_return
      pcustom_langParser->assignment_operator(pcustom_langParser)
 *  - 
 custom_langParser_conditional_expression_return
      pcustom_langParser->conditional_expression(pcustom_langParser)
 *  - 
 custom_langParser_logical_or_expression_return
      pcustom_langParser->logical_or_expression(pcustom_langParser)
 *  - 
 custom_langParser_logical_and_expression_return
      pcustom_langParser->logical_and_expression(pcustom_langParser)
 *  - 
 custom_langParser_inclusive_or_expression_return
      pcustom_langParser->inclusive_or_expression(pcustom_langParser)
 *  - 
 custom_langParser_exclusive_or_expression_return
      pcustom_langParser->exclusive_or_expression(pcustom_langParser)
 *  - 
 custom_langParser_and_expression_return
      pcustom_langParser->and_expression(pcustom_langParser)
 *  - 
 custom_langParser_equality_expression_return
      pcustom_langParser->equality_expression(pcustom_langParser)
 *  - 
 custom_langParser_relational_expression_return
      pcustom_langParser->relational_expression(pcustom_langParser)
 *  - 
 custom_langParser_shift_expression_return
      pcustom_langParser->shift_expression(pcustom_langParser)
 *  - 
 custom_langParser_slice_return
      pcustom_langParser->slice(pcustom_langParser)
 *  - 
 custom_langParser_place_return
      pcustom_langParser->place(pcustom_langParser)
 *  - 
 custom_langParser_rangeList_return
      pcustom_langParser->rangeList(pcustom_langParser)
 *  - 
 custom_langParser_range_return
      pcustom_langParser->range(pcustom_langParser)
 *  - 
 custom_langParser_literal_return
      pcustom_langParser->literal(pcustom_langParser)
 *  - 
 custom_langParser_binOp_return
      pcustom_langParser->binOp(pcustom_langParser)
 *  - 
 custom_langParser_unOp_return
      pcustom_langParser->unOp(pcustom_langParser)
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 *
 * The return type for any particular rule is of course determined by the source
 * grammar file.
 */
// [The "BSD license"]
// Copyright (c) 2005-2009 Jim Idle, Temporal Wave LLC
// http://www.temporal-wave.com
// http://www.linkedin.com/in/jimidle
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef	_custom_langParser_H
#define _custom_langParser_H
/* =============================================================================
 * Standard antlr3 C runtime definitions
 */
#include    <antlr3.h>

/* End of standard antlr 3 runtime definitions
 * =============================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

// Forward declare the context typedef so that we can use it before it is
// properly defined. Delegators and delegates (from import statements) are
// interdependent and their context structures contain pointers to each other
// C only allows such things to be declared if you pre-declare the typedef.
//
typedef struct custom_langParser_Ctx_struct custom_langParser, * pcustom_langParser;



#ifdef	ANTLR3_WINDOWS
// Disable: Unreferenced parameter,							- Rules with parameters that are not used
//          constant conditional,							- ANTLR realizes that a prediction is always true (synpred usually)
//          initialized but unused variable					- tree rewrite variables declared but not needed
//          Unreferenced local variable						- lexer rule declares but does not always use _type
//          potentially unitialized variable used			- retval always returned from a rule
//			unreferenced local function has been removed	- susually getTokenNames or freeScope, they can go without warnigns
//
// These are only really displayed at warning level /W4 but that is the code ideal I am aiming at
// and the codegen must generate some of these warnings by necessity, apart from 4100, which is
// usually generated when a parser rule is given a parameter that it does not use. Mostly though
// this is a matter of orthogonality hence I disable that one.
//
#pragma warning( disable : 4100 )
#pragma warning( disable : 4101 )
#pragma warning( disable : 4127 )
#pragma warning( disable : 4189 )
#pragma warning( disable : 4505 )
#pragma warning( disable : 4701 )
#endif

/* ========================
 * BACKTRACKING IS ENABLED
 * ========================
 */

typedef struct custom_langParser_source_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_source_return;



typedef struct custom_langParser_sourceItem_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_sourceItem_return;



typedef struct custom_langParser_type_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_type_return;



typedef struct custom_langParser_basicType_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_basicType_return;



typedef struct custom_langParser_typeRef_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_typeRef_return;



typedef struct custom_langParser_arrayType_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_arrayType_return;



typedef struct custom_langParser_funcSignature_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_funcSignature_return;



typedef struct custom_langParser_argList_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_argList_return;



typedef struct custom_langParser_arg_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_arg_return;



typedef struct custom_langParser_funcDef_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_funcDef_return;



typedef struct custom_langParser_basicStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_basicStatement_return;



typedef struct custom_langParser_statement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_statement_return;



typedef struct custom_langParser_simpleStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_simpleStatement_return;



typedef struct custom_langParser_repeatStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_repeatStatement_return;



typedef struct custom_langParser_ifStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_ifStatement_return;



typedef struct custom_langParser_loopStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_loopStatement_return;



typedef struct custom_langParser_breakStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_breakStatement_return;



typedef struct custom_langParser_expressionStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_expressionStatement_return;



typedef struct custom_langParser_blockStatement_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_blockStatement_return;



typedef struct custom_langParser_argument_expression_list_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_argument_expression_list_return;



typedef struct custom_langParser_additive_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_additive_expression_return;



typedef struct custom_langParser_multiplicative_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_multiplicative_expression_return;



typedef struct custom_langParser_unary_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_unary_expression_return;



typedef struct custom_langParser_postfix_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_postfix_expression_return;



typedef struct custom_langParser_array_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_array_expression_return;



typedef struct custom_langParser_primary_expression_list_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_primary_expression_list_return;



typedef struct custom_langParser_unary_operator_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_unary_operator_return;



typedef struct custom_langParser_primary_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_primary_expression_return;



typedef struct custom_langParser_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_expression_return;



typedef struct custom_langParser_constant_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_constant_expression_return;



typedef struct custom_langParser_assignment_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_assignment_expression_return;



typedef struct custom_langParser_lvalue_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_lvalue_return;



typedef struct custom_langParser_assignment_operator_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_assignment_operator_return;



typedef struct custom_langParser_conditional_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_conditional_expression_return;



typedef struct custom_langParser_logical_or_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_logical_or_expression_return;



typedef struct custom_langParser_logical_and_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_logical_and_expression_return;



typedef struct custom_langParser_inclusive_or_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_inclusive_or_expression_return;



typedef struct custom_langParser_exclusive_or_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_exclusive_or_expression_return;



typedef struct custom_langParser_and_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_and_expression_return;



typedef struct custom_langParser_equality_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_equality_expression_return;



typedef struct custom_langParser_relational_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_relational_expression_return;



typedef struct custom_langParser_shift_expression_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_shift_expression_return;



typedef struct custom_langParser_slice_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_slice_return;



typedef struct custom_langParser_place_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_place_return;



typedef struct custom_langParser_rangeList_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_rangeList_return;



typedef struct custom_langParser_range_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_range_return;



typedef struct custom_langParser_literal_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_literal_return;



typedef struct custom_langParser_binOp_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_binOp_return;



typedef struct custom_langParser_unOp_return_struct
{
    /** Generic return elements for ANTLR3 rules that are not in tree parsers or returning trees
     */
    pANTLR3_COMMON_TOKEN    start;
    pANTLR3_COMMON_TOKEN    stop;
    pANTLR3_BASE_TREE	tree;

}
    custom_langParser_unOp_return;




/** Context tracking structure for 
custom_langParser

 */
struct custom_langParser_Ctx_struct
{
    /** Built in ANTLR3 context tracker contains all the generic elements
     *  required for context tracking.
     */
    pANTLR3_PARSER   pParser;

     custom_langParser_source_return
     (*source)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_sourceItem_return
     (*sourceItem)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_type_return
     (*type)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_basicType_return
     (*basicType)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_typeRef_return
     (*typeRef)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_arrayType_return
     (*arrayType)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_funcSignature_return
     (*funcSignature)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_argList_return
     (*argList)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_arg_return
     (*arg)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_funcDef_return
     (*funcDef)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_basicStatement_return
     (*basicStatement)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_statement_return
     (*statement)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_simpleStatement_return
     (*simpleStatement)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_repeatStatement_return
     (*repeatStatement)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_ifStatement_return
     (*ifStatement)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_loopStatement_return
     (*loopStatement)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_breakStatement_return
     (*breakStatement)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_expressionStatement_return
     (*expressionStatement)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_blockStatement_return
     (*blockStatement)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_argument_expression_list_return
     (*argument_expression_list)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_additive_expression_return
     (*additive_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_multiplicative_expression_return
     (*multiplicative_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_unary_expression_return
     (*unary_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_postfix_expression_return
     (*postfix_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_array_expression_return
     (*array_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_primary_expression_list_return
     (*primary_expression_list)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_unary_operator_return
     (*unary_operator)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_primary_expression_return
     (*primary_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_expression_return
     (*expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_constant_expression_return
     (*constant_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_assignment_expression_return
     (*assignment_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_lvalue_return
     (*lvalue)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_assignment_operator_return
     (*assignment_operator)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_conditional_expression_return
     (*conditional_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_logical_or_expression_return
     (*logical_or_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_logical_and_expression_return
     (*logical_and_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_inclusive_or_expression_return
     (*inclusive_or_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_exclusive_or_expression_return
     (*exclusive_or_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_and_expression_return
     (*and_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_equality_expression_return
     (*equality_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_relational_expression_return
     (*relational_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_shift_expression_return
     (*shift_expression)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_slice_return
     (*slice)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_place_return
     (*place)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_rangeList_return
     (*rangeList)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_range_return
     (*range)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_literal_return
     (*literal)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_binOp_return
     (*binOp)	(struct custom_langParser_Ctx_struct * ctx);

     custom_langParser_unOp_return
     (*unOp)	(struct custom_langParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred16_custom_lang)	(struct custom_langParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred20_custom_lang)	(struct custom_langParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred21_custom_lang)	(struct custom_langParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred23_custom_lang)	(struct custom_langParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred39_custom_lang)	(struct custom_langParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred40_custom_lang)	(struct custom_langParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred41_custom_lang)	(struct custom_langParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred42_custom_lang)	(struct custom_langParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred43_custom_lang)	(struct custom_langParser_Ctx_struct * ctx);

     ANTLR3_BOOLEAN
     (*synpred55_custom_lang)	(struct custom_langParser_Ctx_struct * ctx);
    // Delegated rules

    const char * (*getGrammarFileName)();
    void            (*reset)  (struct custom_langParser_Ctx_struct * ctx);
    void	    (*free)   (struct custom_langParser_Ctx_struct * ctx);
/* @headerFile.members() */
pANTLR3_BASE_TREE_ADAPTOR	adaptor;
pANTLR3_VECTOR_FACTORY		vectors;
/* End @headerFile.members() */
};

// Function protoypes for the constructor functions that external translation units
// such as delegators and delegates may wish to call.
//
ANTLR3_API pcustom_langParser custom_langParserNew         (
pANTLR3_COMMON_TOKEN_STREAM
 instream);
ANTLR3_API pcustom_langParser custom_langParserNewSSD      (
pANTLR3_COMMON_TOKEN_STREAM
 instream, pANTLR3_RECOGNIZER_SHARED_STATE state);

/** Symbolic definitions of all the tokens that the 
parser
 will work with.
 * \{
 *
 * Antlr will define EOF, but we can't use that as it it is too common in
 * in C header files and that would be confusing. There is no way to filter this out at the moment
 * so we just undef it here for now. That isn't the value we get back from C recognizers
 * anyway. We are looking for ANTLR3_TOKEN_EOF.
 */
#ifdef	EOF
#undef	EOF
#endif
#ifdef	Tokens
#undef	Tokens
#endif
#define EOF      -1
#define T__61      61
#define T__62      62
#define T__63      63
#define T__64      64
#define T__65      65
#define T__66      66
#define T__67      67
#define T__68      68
#define T__69      69
#define T__70      70
#define T__71      71
#define T__72      72
#define T__73      73
#define T__74      74
#define T__75      75
#define T__76      76
#define T__77      77
#define T__78      78
#define T__79      79
#define T__80      80
#define T__81      81
#define T__82      82
#define T__83      83
#define T__84      84
#define T__85      85
#define T__86      86
#define T__87      87
#define T__88      88
#define T__89      89
#define T__90      90
#define T__91      91
#define T__92      92
#define T__93      93
#define T__94      94
#define T__95      95
#define T__96      96
#define T__97      97
#define T__98      98
#define T__99      99
#define T__100      100
#define T__101      101
#define T__102      102
#define T__103      103
#define ADD      4
#define AND      5
#define ARG      6
#define ARG_LIST      7
#define ARRAY      8
#define ARRAY_INIT      9
#define ASSIGN      10
#define ASSIGNMENT_EXPR      11
#define BINARY_EXPR      12
#define BITS      13
#define BITS_TOKEN      14
#define BLOCK_STATEMENT      15
#define BOOL      16
#define BOOL_TOKEN      17
#define BRACES_EXPR      18
#define BREAK_STATEMENT      19
#define BUILTIN      20
#define CALL_EXPR      21
#define CHAR      22
#define CHAR_TOKEN      23
#define CUSTOM      24
#define DEC      25
#define DEC_TOKEN      26
#define DIV      27
#define EQ      28
#define EXPR      29
#define EXPRESSION_STATEMENT      30
#define FUNC_DEF      31
#define FUNC_SIGNATURE      32
#define GT      33
#define GTE      34
#define HEX      35
#define HEX_TOKEN      36
#define IDENTIFIER      37
#define IDENTIFIER_TOKEN      38
#define IF_STATEMENT      39
#define INDEX_EXPR      40
#define LITERAL_EXPR      41
#define LOOP_STATEMENT      42
#define LTS      43
#define LTSE      44
#define MOD      45
#define MUL      46
#define NEQ      47
#define OR      48
#define PLACE_EXPR      49
#define RANGE_LIST      50
#define REPEAT_STATEMENT      51
#define SLICE_EXPR      52
#define SOURCE_ITEM      53
#define STATEMENT      54
#define STRING      55
#define STRING_TOKEN      56
#define SUB      57
#define TYPE_REF      58
#define UNARY_EXPR      59
#define WS      60
#ifdef	EOF
#undef	EOF
#define	EOF	ANTLR3_TOKEN_EOF
#endif

#ifndef TOKENSOURCE
#define TOKENSOURCE(lxr) lxr->pLexer->rec->state->tokSource
#endif

/* End of token definitions for custom_langParser
 * =============================================================================
 */
/** } */

#ifdef __cplusplus
}
#endif

#endif

/* END - Note:Keep extra line feed to satisfy UNIX systems */
