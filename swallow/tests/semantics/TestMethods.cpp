/* TestMethods.cpp --
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
#include "../utils.h"
#include "semantics/Symbol.h"
#include "semantics/ScopedNodes.h"
#include "semantics/Type.h"
#include "common/Errors.h"
#include "semantics/GenericArgument.h"

using namespace Swallow;
using namespace std;

TEST(TestMethods, InstanceMethods)
{
    SEMANTIC_ANALYZE(L"class Counter {\n"
            L"    var count = 0\n"
            L"    func increment() {\n"
            L"        count++\n"
            L"    }\n"
            L"    func incrementBy(amount: Int) {\n"
            L"        count = count + amount\n"
            L"    }\n"
            L"    func reset() {\n"
            L"        count = 0\n"
            L"    }\n"
            L"}\n"
            L"let counter = Counter()\n"
            L"// the initial counter value is 0\n"
            L"counter.increment()\n"
            L"// the counter's value is now 1\n"
            L"counter.incrementBy(5)\n"
            L"// the counter's value is now 6\n"
            L"counter.reset()\n"
            L"// the counter's value is now 0");
    ASSERT_NO_ERRORS();
}

TEST(TestMethods, ChangeFieldWithoutMutating)
{
    SEMANTIC_ANALYZE(L"struct XX\n"
        L"{\n"
        L"    var A = 3\n"
        L"    func foo()\n"
        L"    {\n"
        L"        self.A = 2;\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_CANNOT_ASSIGN_TO_A_IN_B_2);
}

TEST(TestMethods, ChangeFieldWithMutating)
{
    SEMANTIC_ANALYZE(L"struct XX\n"
                L"{\n"
                L"    var A = 3\n"
                L"    mutating func foo()\n"
                L"    {\n"
                L"        self.A = 2;\n"
                L"    }\n"
                L"}");
    ASSERT_NO_ERRORS();
}

TEST(TestMethods, ChangeFieldWithoutMutating_Enum)
{
    SEMANTIC_ANALYZE(L"enum XX\n"
        L"{\n"
        L"    case AA"
        L"    func foo()\n"
        L"    {\n"
        L"        self = XX.AA;\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_CANNOT_ASSIGN_TO_A_IN_A_METHOD_1);
}

TEST(TestMethods, ChangeFieldWithMutating_Enum)
{
    SEMANTIC_ANALYZE(L"enum XX\n"
        L"{\n"
        L"    case AA"
        L"    mutating func foo()\n"
        L"    {\n"
        L"        self = XX.AA;\n"
        L"    }\n"
        L"}");
    ASSERT_NO_ERRORS();
}


TEST(TestMethods, BothMutatingAndNonMutating)
{
    SEMANTIC_ANALYZE(L"struct XX\n"
        L"{\n"
        L"    mutating nonmutating func foo()\n"
        L"    {\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_METHOD_MAY_NOT_BE_DECLARED_BOTH_MUTATING_AND_NONMUTATING);
}

TEST(TestMethods, StaticNonmutating)
{
    SEMANTIC_ANALYZE(L"struct XX\n"
        L"{\n"
        L"    static nonmutating func foo()\n"
        L"    {\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_STATIC_FUNCTIONS_MAY_NOT_BE_DECLARED_A_1);
    ASSERT_EQ(L"nonmutating", error->items[0]);
}

TEST(TestMethods, StaticMutating)
{
    SEMANTIC_ANALYZE(L"struct XX\n"
        L"{\n"
        L"    static mutating func foo()\n"
        L"    {\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_STATIC_FUNCTIONS_MAY_NOT_BE_DECLARED_A_1);
    ASSERT_EQ(L"mutating", error->items[0]);
}


TEST(TestMethods, MutatingVar)
{
    SEMANTIC_ANALYZE(L"mutating var a = 3");
    ASSERT_ERROR(Errors::E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2);
    ASSERT_EQ(L"mutating", error->items[0]);
    ASSERT_EQ(L"func", error->items[1]);
}

TEST(TestMethods, MutatingClass)
{
    SEMANTIC_ANALYZE(L"mutating class A{}");
    ASSERT_ERROR(Errors::E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2);
    ASSERT_EQ(L"mutating", error->items[0]);
    ASSERT_EQ(L"func", error->items[1]);
}

TEST(TestMethods, MutatingStruct)
{
    SEMANTIC_ANALYZE(L"mutating struct A{}");
    ASSERT_ERROR(Errors::E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2);
    ASSERT_EQ(L"mutating", error->items[0]);
    ASSERT_EQ(L"func", error->items[1]);
}


TEST(TestMethods, MutatingProtocol)
{
    SEMANTIC_ANALYZE(L"mutating protocol A{}");
    ASSERT_ERROR(Errors::E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2);
    ASSERT_EQ(L"mutating", error->items[0]);
    ASSERT_EQ(L"func", error->items[1]);
}

TEST(TestMethods, MutatingEnum)
{
    SEMANTIC_ANALYZE(L"mutating enum A{}");
    ASSERT_ERROR(Errors::E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2);
    ASSERT_EQ(L"mutating", error->items[0]);
    ASSERT_EQ(L"func", error->items[1]);
}


TEST(TestMethods, MutatingFunc)
{
    SEMANTIC_ANALYZE(L"mutating func foo() {}");
    ASSERT_ERROR(Errors::E_A_IS_ONLY_VALID_ON_METHODS_1);
    ASSERT_EQ(L"mutating", error->items[0]);
}


TEST(TestMethods, NonMutatingFunc)
{
    SEMANTIC_ANALYZE(L"nonmutating func foo() {}");
    ASSERT_ERROR(Errors::E_A_IS_ONLY_VALID_ON_METHODS_1);
    ASSERT_EQ(L"nonmutating", error->items[0]);
}
TEST(TestMethods, MutatingClassFunc)
{
    SEMANTIC_ANALYZE(L"class Class\n"
        L"{\n"
        L"    mutating func foo()\n"
        L"    {\n"
        L"        \n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_A_ISNT_VALID_ON_METHODS_IN_CLASSES_OR_CLASS_BOUND_PROTOCOLS);
    ASSERT_EQ(L"mutating", error->items[0]);
}

TEST(TestMethods, ModifyValuesFromInstanceMethods)
{
    SEMANTIC_ANALYZE(L"struct Point {\n"
        L"    var x = 0.0, y = 0.0\n"
        L"    mutating func moveByX(deltaX: Double, y deltaY: Double) {\n"
        L"        x = x + deltaX\n"
        L"        y = y + deltaY\n"
        L"    }\n"
        L"}\n"
        L"var somePoint = Point(x: 1.0, y: 1.0)\n"
        L"somePoint.moveByX(2.0, y: 3.0)\n"
        L"println(\"The point is now at (\\(somePoint.x), \\(somePoint.y))\")");
    ASSERT_NO_ERRORS();
}


TEST(TestMethods, ModifyValuesFromInstanceMethods_Let)
{
    SEMANTIC_ANALYZE(L"struct Point {\n"
        L"    var x = 0.0, y = 0.0\n"
        L"    mutating func moveByX(deltaX: Double, y deltaY: Double) {\n"
        L"        x = x + deltaX\n"
        L"        y = y + deltaY\n"
        L"    }\n"
        L"}\n"
        L"let fixedPoint = Point(x: 3.0, y: 3.0)\n"
        L"fixedPoint.moveByX(2.0, y: 3.0)\n");
    ASSERT_ERROR(Errors::E_IMMUTABLE_VALUE_OF_TYPE_A_ONLY_HAS_MUTATING_MEMBERS_NAMED_B_2);
}



TEST(TestMethods, ModifyValuesFromInstanceMethods_NonmutatingSelf)
{
    SEMANTIC_ANALYZE(
        L"struct TTT\n"
        L"{\n"
        L"    var x = 3\n"
        L"    mutating func foo()\n"
        L"    {\n"
        L"    }\n"
        L"    func xxx()\n"
        L"    {\n"
        L"        foo()\n"
        L"        \n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_IMMUTABLE_VALUE_OF_TYPE_A_ONLY_HAS_MUTATING_MEMBERS_NAMED_B_2);
}

TEST(TestMethods, AssigningToSelfWithinMutatingMethod)
{
    SEMANTIC_ANALYZE(L"struct Point {\n"
        L"    var x = 0.0, y = 0.0\n"
        L"    mutating func moveByX(deltaX: Double, y deltaY: Double) {\n"
        L"        self = Point(x: x + deltaX, y: y + deltaY)\n"
        L"    }\n"
        L"}");
    ASSERT_NO_ERRORS();
}

TEST(TestMethods, AssigningToSelfWithinMutatingMethod2)
{
    SEMANTIC_ANALYZE(L"enum TriStateSwitch {\n"
        L"    case Off, Low, High\n"
        L"    mutating func next() {\n"
        L"        switch self {\n"
        L"        case Off:\n"
        L"            self = Low\n"
        L"        case Low:\n"
        L"            self = High\n"
        L"        case High:\n"
        L"            self = Off\n"
        L"        }\n"
        L"    }\n"
        L"}\n"
        L"var ovenLight = TriStateSwitch.Low\n"
        L"ovenLight.next()\n"
        L"ovenLight.next()");
    ASSERT_NO_ERRORS();
}

TEST(TestMethods, TypeMethods)
{
    SEMANTIC_ANALYZE(L"class SomeClass {\n"
        L"    static func someTypeMethod() {\n"
        L"        // type method implementation goes here\n"
        L"    }\n"
        L"}\n"
        L"SomeClass.someTypeMethod()");
    ASSERT_ERROR(Errors::E_STATIC_PROPERTIES_ARE_ONLY_ALLOWED_WITHIN_STRUCTS_AND_ENUMS);
}


TEST(TestMethods, TypeMethods2)
{
    SEMANTIC_ANALYZE(L"class SomeClass {\n"
        L"    class func someTypeMethod() {\n"
        L"        // type method implementation goes here\n"
        L"    }\n"
        L"}\n"
        L"SomeClass.someTypeMethod()");
    ASSERT_NO_ERRORS();
}

TEST(TestMethods, TypeMethods3)
{
    SEMANTIC_ANALYZE(L"class func test()\n"
        L"{\n"
        L"    \n"
        L"}");
    ASSERT_ERROR(Errors::E_CLASS_PROPERTIES_MAY_ONLY_BE_DECLARED_ON_A_TYPE);
}

TEST(TestMethods, TypeMethods4)
{
    SEMANTIC_ANALYZE(L"struct LevelTracker {\n"
        L"    static var highestUnlockedLevel = 1\n"
        L"    static func unlockLevel(level: Int) {\n"
        L"        if level > highestUnlockedLevel { highestUnlockedLevel = level }\n"
        L"    }\n"
        L"    static func levelIsUnlocked(level: Int) -> Bool {\n"
        L"        return level <= highestUnlockedLevel\n"
        L"    }\n"
        L"    var currentLevel = 1\n"
        L"    mutating func advanceToLevel(level: Int) -> Bool {\n"
        L"        if LevelTracker.levelIsUnlocked(level) {\n"
        L"            currentLevel = level\n"
        L"            return true\n"
        L"        } else {\n"
        L"            return false\n"
        L"        }\n"
        L"    }\n"
        L"}\n"
        L"class Player {\n"
        L"    var tracker = LevelTracker()\n"
        L"    let playerName: String\n"
        L"    func completedLevel(level: Int) {\n"
        L"        LevelTracker.unlockLevel(level + 1)\n"
        L"        tracker.advanceToLevel(level + 1)\n"
        L"    }\n"
        L"    init(name: String) {\n"
        L"        playerName = name\n"
        L"    }\n"
        L"}\n"
        L"var player = Player(name: \"Argyrios\")\n"
        L"player.completedLevel(1)\n"
        L"println(\"highest unlocked level is now \\(LevelTracker.highestUnlockedLevel)\")\n"
        L"player = Player(name: \"Beto\")\n"
        L"if player.tracker.advanceToLevel(6) {\n"
        L"    println(\"player is now on level 6\")\n"
        L"} else {\n"
        L"    println(\"level 6 has not yet been unlocked\")\n"
        L"}");
    ASSERT_NO_ERRORS();
}


TEST(TestMethods, StructInitLet)
{
    SEMANTIC_ANALYZE(L"struct test\n"
        L"{\n"
        L"    let a = 3\n"
        L"    init()\n"
        L"    {\n"
        L"        a = 5\n"
        L"    }\n"
        L"    \n"
        L"}");
    ASSERT_NO_ERRORS();
}


TEST(TestMethods, StructInitSelfLet)
{
    SEMANTIC_ANALYZE(L"struct test\n"
        L"{\n"
        L"    let a = 3\n"
        L"    init()\n"
        L"    {\n"
        L"        self.a = 5\n"
        L"    }\n"
        L"    \n"
        L"}");
    ASSERT_NO_ERRORS();
}


TEST(TestMethods, MissingReturn)
{
    SEMANTIC_ANALYZE(L"func a() -> Int\n"
            L"{\n"
            L"    \n"
            L"}");
    ASSERT_ERROR(Errors::E_MISSING_RETURN_IN_A_FUNCTION_EXPECTED_TO_RETURN_A_1);
    ASSERT_EQ(L"Int", error->items[0]);
}
TEST(TestMethods, MissingReturn2)
{
    SEMANTIC_ANALYZE(L"func a(f : Bool) -> Int\n"
            L"{\n"
            L"    if(f)\n"
            L"    {\n"
            L"        return 3\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_MISSING_RETURN_IN_A_FUNCTION_EXPECTED_TO_RETURN_A_1);
    ASSERT_EQ(L"Int", error->items[0]);
}
TEST(TestMethods, MissingReturn3)
{
    SEMANTIC_ANALYZE(L"func a(f : Bool) -> Int\n"
            L"{\n"
            L"    if(f)\n"
            L"    {\n"
            L"        return 3\n"
            L"    }\n"
            L"    else\n"
            L"    {\n"
            L"        return 3\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestMethods, MissingReturn4)
{
    SEMANTIC_ANALYZE(L"func a(f : Bool) -> Int\n"
            L"{\n"
            L"    if(f)\n"
            L"    {\n"
            L"        return 1\n"
            L"    }\n"
            L"    else\n"
            L"    {\n"
            L"        return 2\n"
            L"    }\n"
            L"    return 3\n"
            L"}");
    ASSERT_ERROR(Errors::W_CODE_AFTER_A_WILL_NEVER_BE_EXECUTED_1);
}
TEST(TestMethods, MissingReturn5)
{
    SEMANTIC_ANALYZE(L"func a(f : Bool) -> Int\n"
            L"{\n"
            L"    if(f)\n"
            L"    {\n"
            L"      return 1\n"
            L"    }\n"
            L"    return 3\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestMethods, MissingReturn6)
{
    SEMANTIC_ANALYZE(L"func a(f : Bool) -> Int\n"
            L"{\n"
            L"    return 1\n"
            L"    var b = 3\n"
            L"    b = 5\n"
            L"    return b\n"
            L"}");
    ASSERT_ERROR(Errors::W_CODE_AFTER_A_WILL_NEVER_BE_EXECUTED_1);
    ASSERT_EQ(4, error->line);
}
