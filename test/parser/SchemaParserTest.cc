// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include "imlab/parser/AST.h"
#include "imlab/parser/ParseContext.h"
#include <sstream>
#include <vector>
#include <gtest/gtest.h>

using Type = imlab::Type;
using ParseContext = imlab::parser::ParseContext;

namespace test {

TEST(ParserTest, SchemaParseEmptyString) {
    std::istringstream in("");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 0);
}

TEST(ParserTest, SchemaParseEmptyTable) {
    std::istringstream in("create table foo ();");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    {
        auto ptr = parseTrees[0].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CreateTableType);
        auto stmtPtr = static_cast<const imlab::parser::CreateTable*>(ptr);
        auto& table = stmtPtr->getTable();
        EXPECT_EQ(table.id, "foo");
        EXPECT_TRUE(table.columns.empty());
        EXPECT_TRUE(table.primary_key.empty());
    }
}

TEST(ParserTest, SchemaParseTwoEmptyTables) {
    std::istringstream in("create table foo (); create table bar ();");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 2);
    {
        auto ptr = parseTrees[0].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CreateTableType);
        auto stmtPtr = static_cast<const imlab::parser::CreateTable*>(ptr);
        auto& table = stmtPtr->getTable();
        EXPECT_EQ(table.id, "foo");
        EXPECT_TRUE(table.columns.empty());
        EXPECT_TRUE(table.primary_key.empty());
    }
    {
        auto ptr = parseTrees[1].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CreateTableType);
        auto stmtPtr = static_cast<const imlab::parser::CreateTable*>(ptr);
        auto& table = stmtPtr->getTable();
        EXPECT_EQ(table.id, "bar");
        EXPECT_TRUE(table.columns.empty());
        EXPECT_TRUE(table.primary_key.empty());
    }
}

TEST(ParserTest, SchemaParseSingleIntegerColumn) {
    std::istringstream in(R"SQLSCHEMA(
        create table foo ( bar integer not null );
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    {
        auto ptr = parseTrees[0].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CreateTableType);
        auto stmtPtr = static_cast<const imlab::parser::CreateTable*>(ptr);
        auto& table = stmtPtr->getTable();
        EXPECT_EQ(table.id, "foo");
        ASSERT_EQ(table.columns.size(), 1);
        EXPECT_EQ(table.columns[0].id, "bar");
        EXPECT_EQ(table.columns[0].type.getClass(), Type::kInteger);
        EXPECT_TRUE(table.primary_key.empty());
    }
}

TEST(ParserTest, SchemaParseSingleTimestampColumn) {
    std::istringstream in(R"SQLSCHEMA(
        create table foo ( bar timestamp not null );
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    {
        auto ptr = parseTrees[0].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CreateTableType);
        auto stmtPtr = static_cast<const imlab::parser::CreateTable*>(ptr);
        auto& table = stmtPtr->getTable();
        EXPECT_EQ(table.id, "foo");
        ASSERT_EQ(table.columns.size(), 1);
        EXPECT_EQ(table.columns[0].id, "bar");
        EXPECT_EQ(table.columns[0].type.getClass(), Type::kTimestamp);
        EXPECT_TRUE(table.primary_key.empty());
    }
}

TEST(ParserTest, SchemaParseSingleNumericColumn) {
    std::istringstream in(R"SQLSCHEMA(
        create table foo ( bar numeric(4, 2) not null );
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    {
        auto ptr = parseTrees[0].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CreateTableType);
        auto stmtPtr = static_cast<const imlab::parser::CreateTable*>(ptr);
        auto& table = stmtPtr->getTable();
        EXPECT_EQ(table.id, "foo");
        ASSERT_EQ(table.columns.size(), 1);
        EXPECT_EQ(table.columns[0].id, "bar");
        EXPECT_EQ(table.columns[0].type.getClass(), Type::kNumeric);
        EXPECT_EQ(table.columns[0].type.getPrecision(), 4);
        EXPECT_EQ(table.columns[0].type.getScale(), 2);
        EXPECT_TRUE(table.primary_key.empty());
    }
}

TEST(ParserTest, SchemaParseSingleCharColumn) {
    std::istringstream in(R"SQLSCHEMA(
        create table foo ( bar char(20) not null );
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    {
        auto ptr = parseTrees[0].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CreateTableType);
        auto stmtPtr = static_cast<const imlab::parser::CreateTable*>(ptr);
        auto& table = stmtPtr->getTable();
        EXPECT_EQ(table.id, "foo");
        ASSERT_EQ(table.columns.size(), 1);
        EXPECT_EQ(table.columns[0].id, "bar");
        EXPECT_EQ(table.columns[0].type.getClass(), Type::kChar);
        EXPECT_EQ(table.columns[0].type.getLength(), 20);
        EXPECT_TRUE(table.primary_key.empty());
    }
}

TEST(ParserTest, SchemaParseSingleVarcharColumn) {
    std::istringstream in(R"SQLSCHEMA(
        create table foo ( bar varchar(20) not null );
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    {
        auto ptr = parseTrees[0].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CreateTableType);
        auto stmtPtr = static_cast<const imlab::parser::CreateTable*>(ptr);
        auto& table = stmtPtr->getTable();
        EXPECT_EQ(table.id, "foo");
        ASSERT_EQ(table.columns.size(), 1);
        EXPECT_EQ(table.columns[0].id, "bar");
        EXPECT_EQ(table.columns[0].type.getClass(), Type::kVarchar);
        EXPECT_EQ(table.columns[0].type.getLength(), 20);
        EXPECT_TRUE(table.primary_key.empty());
    }
}

TEST(ParserTest, SchemaParseMultipleColumns) {
    std::istringstream in(R"SQLSCHEMA(
        create table foo (
            c1 integer not null,
            c2 timestamp not null,
            c3 numeric(4, 2) not null,
            c4 char(20) not null,
            c5 varchar(20) not null
        );
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    {
        auto ptr = parseTrees[0].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CreateTableType);
        auto stmtPtr = static_cast<const imlab::parser::CreateTable*>(ptr);
        auto& table = stmtPtr->getTable();
        EXPECT_EQ(table.id, "foo");
        ASSERT_EQ(table.columns.size(), 5);
        EXPECT_EQ(table.columns[0].id, "c1");
        EXPECT_EQ(table.columns[2].id, "c3");
        EXPECT_EQ(table.columns[3].id, "c4");
        EXPECT_EQ(table.columns[4].id, "c5");
        EXPECT_EQ(table.columns[0].type.getClass(), Type::kInteger);
        EXPECT_EQ(table.columns[1].type.getClass(), Type::kTimestamp);
        EXPECT_EQ(table.columns[2].type.getClass(), Type::kNumeric);
        EXPECT_EQ(table.columns[3].type.getClass(), Type::kChar);
        EXPECT_EQ(table.columns[4].type.getClass(), Type::kVarchar);
        EXPECT_EQ(table.columns[2].type.getPrecision(), 4);
        EXPECT_EQ(table.columns[2].type.getScale(), 2);
        EXPECT_EQ(table.columns[3].type.getLength(), 20);
        EXPECT_EQ(table.columns[4].type.getLength(), 20);
        EXPECT_TRUE(table.primary_key.empty());
    }
}

TEST(ParserTest, SchemaParseMultipleTables) {
    std::istringstream in(R"SQLSCHEMA(
        create table foo (
            f1 integer not null,
            f2 timestamp not null,
            f3 numeric(4, 2) not null,
            f4 char(20) not null,
            f5 varchar(20) not null
        );

        create table bar (
            b1 integer not null,
            b2 timestamp not null,
            b3 numeric(4, 2) not null,
            b4 char(20) not null,
            b5 varchar(20) not null
        );
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 2);
    {
        auto ptr = parseTrees[0].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CreateTableType);
        auto stmtPtr = static_cast<const imlab::parser::CreateTable*>(ptr);
        auto& table = stmtPtr->getTable();
        EXPECT_EQ(table.id, "foo");
        ASSERT_EQ(table.columns.size(), 5);
        EXPECT_EQ(table.columns[0].id, "f1");
        EXPECT_EQ(table.columns[2].id, "f3");
        EXPECT_EQ(table.columns[3].id, "f4");
        EXPECT_EQ(table.columns[4].id, "f5");
        EXPECT_EQ(table.columns[0].type.getClass(), Type::kInteger);
        EXPECT_EQ(table.columns[1].type.getClass(), Type::kTimestamp);
        EXPECT_EQ(table.columns[2].type.getClass(), Type::kNumeric);
        EXPECT_EQ(table.columns[3].type.getClass(), Type::kChar);
        EXPECT_EQ(table.columns[4].type.getClass(), Type::kVarchar);
        EXPECT_EQ(table.columns[2].type.getPrecision(), 4);
        EXPECT_EQ(table.columns[2].type.getScale(), 2);
        EXPECT_EQ(table.columns[3].type.getLength(), 20);
        EXPECT_EQ(table.columns[4].type.getLength(), 20);
        EXPECT_TRUE(table.primary_key.empty());
    }
    {
        auto ptr = parseTrees[1].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CreateTableType);
        auto stmtPtr = static_cast<const imlab::parser::CreateTable*>(ptr);
        auto& table = stmtPtr->getTable();
        EXPECT_EQ(table.id, "bar");
        ASSERT_EQ(table.columns.size(), 5);
        EXPECT_EQ(table.columns[0].id, "b1");
        EXPECT_EQ(table.columns[2].id, "b3");
        EXPECT_EQ(table.columns[3].id, "b4");
        EXPECT_EQ(table.columns[4].id, "b5");
        EXPECT_EQ(table.columns[0].type.getClass(), Type::kInteger);
        EXPECT_EQ(table.columns[1].type.getClass(), Type::kTimestamp);
        EXPECT_EQ(table.columns[2].type.getClass(), Type::kNumeric);
        EXPECT_EQ(table.columns[3].type.getClass(), Type::kChar);
        EXPECT_EQ(table.columns[4].type.getClass(), Type::kVarchar);
        EXPECT_EQ(table.columns[2].type.getPrecision(), 4);
        EXPECT_EQ(table.columns[2].type.getScale(), 2);
        EXPECT_EQ(table.columns[3].type.getLength(), 20);
        EXPECT_EQ(table.columns[4].type.getLength(), 20);
        EXPECT_TRUE(table.primary_key.empty());
    }
}

TEST(ParserTest, SchemaParsePrimaryKey) {
    std::istringstream in(R"SQLSCHEMA(
        create table foo (
            c1 integer not null,
            c2 timestamp not null,
            c3 numeric(4, 2) not null,
            c4 char(20) not null,
            c5 varchar(20) not null,
            primary key (c1)
        );
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    {
        auto ptr = parseTrees[0].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CreateTableType);
        auto stmtPtr = static_cast<const imlab::parser::CreateTable*>(ptr);
        auto& table = stmtPtr->getTable();
        EXPECT_EQ(table.id, "foo");
        ASSERT_EQ(table.columns.size(), 5);
        ASSERT_EQ(table.primary_key.size(), 1);
        EXPECT_EQ(table.primary_key[0].id, "c1");
    }
}

TEST(ParserTest, SchemaParseComposedPrimaryKey) {
    std::istringstream in(R"SQLSCHEMA(
        create table foo (
            c1 integer not null,
            c2 timestamp not null,
            c3 numeric(4, 2) not null,
            c4 char(20) not null,
            c5 varchar(20) not null,
            primary key (c1, c2)
        );
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    {
        auto ptr = parseTrees[0].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CreateTableType);
        auto stmtPtr = static_cast<const imlab::parser::CreateTable*>(ptr);
        auto& table = stmtPtr->getTable();
        EXPECT_EQ(table.id, "foo");
        ASSERT_EQ(table.columns.size(), 5);
        ASSERT_EQ(table.primary_key.size(), 2);
        EXPECT_EQ(table.primary_key[0].id, "c1");
        EXPECT_EQ(table.primary_key[1].id, "c2");
    }
}

TEST(ParserTest, CopyWithoutDelimiter) {
    std::istringstream in(R"SQLSCHEMA(
        copy foo from 'bar/foo';
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    {
        auto ptr = parseTrees[0].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CopyTableType);
        auto stmtPtr = static_cast<const imlab::parser::CopyTable*>(ptr);
        EXPECT_EQ(stmtPtr->getTable(), "foo");
        EXPECT_EQ(stmtPtr->getFile(), "bar/foo");
        EXPECT_EQ(stmtPtr->getDelimiter(), "|");
    }
}

TEST(ParserTest, CopyWitDelimiter) {
    std::istringstream in(R"SQLSCHEMA(
        copy foo from 'bar/foo' delimiter ',';
    )SQLSCHEMA");
    ParseContext spc;
    auto parseTrees = spc.Parse(in);
    ASSERT_EQ(parseTrees.size(), 1);
    {
        auto ptr = parseTrees[0].get();
        ASSERT_EQ(ptr->getType(), imlab::parser::AST::ASTType::CopyTableType);
        auto stmtPtr = static_cast<const imlab::parser::CopyTable*>(ptr);
        EXPECT_EQ(stmtPtr->getTable(), "foo");
        EXPECT_EQ(stmtPtr->getFile(), "bar/foo");
        EXPECT_EQ(stmtPtr->getDelimiter(), ",");
    }
}

} // namespace test
