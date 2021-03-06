/* OperatorResolver.h --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef OPERATOR_RESOLVER_H
#define OPERATOR_RESOLVER_H
#include "SemanticPass.h"

SWALLOW_NS_BEGIN

class SymbolRegistry;
class CompilerResults;

class SWALLOW_EXPORT OperatorResolver : public SemanticPass
{
public:
    OperatorResolver(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults);
public:
    virtual void visitAssignment(const AssignmentPtr& node) override;
    virtual void visitComputedProperty(const ComputedPropertyPtr& node) override;
    virtual void visitProtocol(const ProtocolDefPtr& node) override;
    virtual void visitClosure(const ClosurePtr& node) override;
    virtual void visitProgram(const ProgramPtr& node) override;
    virtual void visitCodeBlock(const CodeBlockPtr& node) override;
public:
    virtual void visitOperator(const OperatorDefPtr& node) override;
    virtual void visitValueBinding(const ValueBindingPtr& node) override;
    virtual void visitStringInterpolation(const StringInterpolationPtr& node);
    virtual void visitConditionalOperator(const ConditionalOperatorPtr& node) override;
    virtual void visitBinaryOperator(const BinaryOperatorPtr& node) override;
    virtual void visitUnaryOperator(const UnaryOperatorPtr& node) override;
    virtual void visitTuple(const TuplePtr& node) override;
    virtual void visitReturn(const ReturnStatementPtr& node) override;
    virtual void visitParenthesizedExpression(const ParenthesizedExpressionPtr& node) override;
    virtual void visitArrayLiteral(const ArrayLiteralPtr& node) override;
    virtual void visitDictionaryLiteral(const DictionaryLiteralPtr& node) override;
    virtual void visitForcedValue(const ForcedValuePtr& node) override;
    virtual void visitOptionalChaining(const OptionalChainingPtr& node) override;
    virtual void visitIf(const IfStatementPtr& node) override;
    virtual void visitSwitchCase(const SwitchCasePtr& node) override;
    virtual void visitCase(const CaseStatementPtr& node) override;

public:
    OperatorPtr sortExpression(const OperatorPtr& op);
    bool rotateRequired(const OperatorPtr& lhs, const OperatorPtr& rhs);
private:
    template<class T, typename Ptr = std::shared_ptr<T>>
    Ptr transform(const Ptr& ptr)
    {
        if(!ptr)
            return nullptr;
        std::static_pointer_cast<Node>(ptr)->accept(this);
        if(OperatorPtr op = std::dynamic_pointer_cast<Operator>(ptr))
        {
            return std::static_pointer_cast<T>(sortExpression(op));
        }
        return ptr;
    }
};

SWALLOW_NS_END




#endif//OPERATOR_RESOLVER_H
