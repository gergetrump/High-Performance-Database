# IMLAB

## Task 1

1. Implement row or column store data structures in [tools/statictpcc/TPCC.h](tools/statictpcc/TPCC.h) for the tables
   specified
   in [schema.sql](data/schema.sql). Use the types specified [here](imlab/types/Types.h). Use appropriate data
   structures for the
   primary keys
2. Implement load functions in [TPCC.cc](tools/statictpcc/TPCC.cc#L12-50) to parse TBL files (= CSV files) and populate
   your data structures with the
   data of [these TPC-C files](https://db.in.tum.de/teaching/ws2122/imlab/tpcc_5w.tar.gz) (please do NOT push them into
   the GitLab).
   Do not forget the primary keys!
3. Implement the [New Order](data/new_order.sql) transaction as a C++ function
   in [TPCC.cc](tools/statictpcc/TPCC.cc#L89) for your
   chosen storage format
4. Implement a REPL (Read Evaluate Print Loop) in [tpcc.cc](tools/tpcc.cc#L74):
    * Read one line from standard input
    * Match the input to one of these cases:
        * A `LOAD` command of the form `LOAD tpcc FROM <directory>`
        * An `EXECUTE` command of the form `EXECUTE neworder`
    * In case of `LOAD`, execute the CSV parsing functions and populate the table data structures with the
      data of the CSV files in the specified directory
    * In case of `EXECUTE`, execute the [New Order](data/new_order.sql) transaction for your chosen storage format and
      print the number of tuples after executing the transaction
    * Else print an error message to standard error
    * Loop :-)
5. Add and extend the tests for your implementation [here](test/tpcc/DatabaseTest.cc)

Can you populate your tables with the TPC-C data in less than 1s? :-)

## Task 2

1. Implement a SQL parser using *bison* and *flex*
    * For this, modify the [scanner](imlab/parser/scanner.l) and [grammar](imlab/parser/parser.y)
    * The parser takes the text input and returns a vector of pointers to the parsed `AST`s
    * The parser must be able to parse the DDL statements of [schema.sql](data/schema.sql) following the grammar
      specified below
    * The parser must also support the parsing of copy statements
2. Implement semantic analysis for the AST of the parser
    * The semantic analysis takes an `AST` and returns a corresponding `Statement`
    * Throw a meaningful [exception](imlab/runtime/RuntimeException.h) if a table of that name already exists in the
      database
    * Throw a meaningful [exception](imlab/runtime/RuntimeException.h) if attributes are not declared `NOT NULL` or are
      declared twice
3. Implement another REPL in `imlabdb.cc`
    * Read one line from standard input
    * Run the parser on the input
    * For each statement of the `AST`:
        * Run the semantic analysis on the parsed AST and return the corresponding Statement
        * Execute the `Statement` returned by the semantic analysis
            * In case of a `CreateStatement`, create the table with its columns in the database
            * In case of a `CopyStatement`, populate the tables with the data of the specified data file. It is
              sufficient to support only the types used in the TPC-C schema
    * Loop :-)

This is the grammar for `CREATE` and `COPY`:

```
CREATE TABLE table_name '('
    [
        column data_type NOT NULL 
        [',' column data_type NOT NULL]...
        [',' PRIMARY_KEY '(' column [',' column]... ')']
    ]
')';

COPY table_name FROM file [DELIMITER delimiter];

where file and delimiter are single quoted string literals, e.g. 'foo'
```

The skeleton for this task contains a [scanner](data/foo/foo_scanner.l) and [grammar](data/foo/foo_parser.y) for a
toy format called **FOO**, which looks like this:

```
foo bar1 {
    a integer,
    b char{20}
};

foo bar2 {
    c integer
};
```

First learn how bison & flex work together and how they are used in the following files:

* [FOO grammar](data/foo/foo_parser.y)
* [FOO scanner](data/foo/foo_scanner.l)
* [CMake integration](data/foo/local.cmake)
* [Foo parse context H](data/foo/foo_parse_context.h)
* [Foo parse context CC](data/foo/foo_parse_context.cc)
* [Foo compiler H](data/foo/foo_compiler.h)
* [Foo compiler CC](data/foo/foo_compiler.cc)

You can use the executable `fooc` to parse a **FOO** file with the following command:

```
./fooc --in data/foo/example.foo --out_cc /tmp/example.cc --out_h /tmp/example.h
```

## Task 3

1. Extend the existing parser to also parse queries for the following subset of SQL:
    ```
    SELECT { '*' | [column [',' column]...] }
    FROM table_name [',' table_name]...
    [ WHERE column '=' { column | constant } [ AND column '=' { column | constant } ]...];
    
    table_name := text text (e.g. students s) | text (e.g. students)
    column := text.text (e.g. s.matrnr) | text (e.g. matrnr)
    ```
   * The select clause contains one or more attribute names.
   * The from clause contains one or more relation names.
   * The where clause is optional and can contain one or more selections (connected by "and").
   * You only need to support selections of the form "attribute = attribute" and "attribute = constant".
2. Extend the semantic analysis for DQL statements:
   * Build an algebra tree for the DQL statement and return it inside the [QueryStatement](imlab/statement/QueryStatement.h)
   * Use the [Print operator](imlab/algebra/Print.h) for the select clause, the [InnerJoin operator](imlab/algebra/InnerJoin.h) and
     [TableScan operator](imlab/algebra/TableScan.h) for the from clause, the [Select operator](imlab/algebra/Select.h) for
     the where clause
   * Throw an exception when non-existing attributes or tables are referenced
   * Build your tree from bottom up: First analyze the from clause, then the where clause, and finally the select
     clause. Build left-deep join trees based on the order of the relations in the from clause
3. Extend the REPL ("read-eval-print-loop") of your database system

Bonus tasks:

* Implement parsing and semantic analysis for subqueries in the from clause, i.e., `'(' SELECT ... ')' [ subquery_name ]`
  as an alternative to `table_name`
* Implement the interface of the [iterator (a.k.a. Volcano) model](https://db.in.tum.de/teaching/ss24/moderndbs/chapter5.pdf),
  i.e., `open`, `next`, `close`, and run some queries

## Task 4

1. Implement code generation from relational algebra trees to C++ code using the [produce/consume (push) model](https://db.in.tum.de/teaching/ss24/moderndbs/chapter6.pdf)
   * You need to support the following operators: [TableScan](imlab/algebra/TableScan.cc), [Select](imlab/algebra/Select.cc), [Print](imlab/algebra/Print.cc) and [InnerJoin](imlab/algebra/InnerJoin.cc)
   * To evaluate expressions, implement `produce` on the classes inheriting from [Expression](imlab/algebra/Expression.cc)
2. Implement code generation, compilation and execution in the [QueryStatement](imlab/statement/QueryStatement.cc):
   1. Call `prepare` and `produce` on the operator tree and write the generated code into a temporary file
   2. Compile the temporary file with the C++ compiler to a shared library (.so or .dylib file), e.g., `g++ -shared -o /tmp/code.so /tmp/code.cc`
   3. Load the shared library using `dlopen`, `dlsym`, and `dlclose` and execute the query.

Bonus tasks:

* Implement code generation for the [CreateTableStatement](imlab/statement/CreateTableStatement.cc) and/or the [CopyTableStatement](imlab/statement/CopyTableStatement.cc)

## Task 5

1. Implement the [LazyMultiMap](imlab/infra/HashTable.h) hashtable
2. Implement predicate push down as an optimization pass for your algebra tree, i.e., shift all select operators as far
   down in the tree as possible, and into joins where applicable
3. Implement hash join for join operators having binary expressions:
   * Consume the left input of the join into a LazyMultiMap hashtable
   * Pass the right input to the parent operator if there is a match in the hashtable

Bonus tasks:

* Support more expressions, like addition.

## Task 6

* Implement [morsel-driven parallelism](https://dl.acm.org/doi/pdf/10.1145/2588555.2610507) in your query engine:
   * Use [tbb](https://github.com/oneapi-src/oneTBB) to parallelize data processing
   * Use `tbb::parallel_for` in your table scans
   * Make your hash table of task 5 thread-safe and build it multi-threaded


Make sure your builds are not failing! <br/>
*Left Sidebar > Build > Pipelines*
