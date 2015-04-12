/* NameMangling.h --
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
#ifndef NAME_MANGLING_H
#define NAME_MANGLING_H
#include "swallow_conf.h"
#include "swallow_types.h"
#include <string>
#include <sstream>
#include <map>

SWALLOW_NS_BEGIN
typedef std::shared_ptr<class Symbol> SymbolPtr;
typedef std::shared_ptr<class SymbolPlaceHolder> SymbolPlaceHolderPtr;
typedef std::shared_ptr<class GenericDefinition> GenericDefinitionPtr;
typedef std::shared_ptr<class Type> TypePtr;
class SymbolRegistry;
struct ManglingContext;

/*!
 * \brief Generate mangled name for a symbol or decode a mangled name into a symbol
 */
class SWALLOW_EXPORT NameMangling
{
public:
    NameMangling(SymbolRegistry* registry);
    ~NameMangling();
public:
    /*!
     * \brief Decode a mangled name into a symbol
     * \param name
     * \return nullptr if failed to decode it
     */
    SymbolPtr decode(const wchar_t* name);

    /*!
     * \brief Encode a symbol into a mangled name
     * \param symbol
     * \return
     */
    std::wstring encode(const SymbolPtr& symbol);
private:
    void encodeType(std::wstringstream& out, const std::wstring& moduleName, const std::wstring& typeName);
    void encodeType(ManglingContext& out, const TypePtr& type, bool wrapCollections = true);
    void defineAbbreviation(const TypePtr&, const std::wstring& abbrev);
    std::wstring encodeVariable(const SymbolPlaceHolderPtr& symbol);

private:
    std::map<TypePtr, std::wstring> typeToName;
    std::map<std::wstring, TypePtr> nameToType;

    void encodeGeneric(ManglingContext &context, const GenericDefinitionPtr &def);
};

SWALLOW_NS_END

#endif//NAME_MANGLING_H
