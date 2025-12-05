# ---------------------------------------------------------------------------
# IMLAB
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Bison & Flex
# ---------------------------------------------------------------------------

# Register flex and bison output
set(FOOC_SCANNER_OUT "${CMAKE_SOURCE_DIR}/data/foo/gen/foo_scanner.cc")
set(FOOC_PARSER_OUT "${CMAKE_SOURCE_DIR}/data/foo/gen/foo_parser.cc")
set(FOOC_COMPILER "${CMAKE_SOURCE_DIR}/data/foo/foo_compiler.cc")
set(FOOC_PARSE_CONTEXT "${CMAKE_SOURCE_DIR}/data/foo/foo_parse_context.cc")
set(FOOC_CC ${FOOC_SCANNER_OUT} ${FOOC_PARSER_OUT} ${FOOC_COMPILER} ${FOOC_PARSE_CONTEXT})
set(FOOC_CC_LINTING ${FOOC_COMPILER} ${FOOC_PARSE_CONTEXT})

# Clear the output files
file(WRITE ${FOOC_SCANNER_OUT} "")
file(WRITE ${FOOC_PARSER_OUT} "")

# Generate parser & scanner
add_custom_target(fooc_parser
        COMMAND ${BISON_EXECUTABLE}
        --defines="${CMAKE_SOURCE_DIR}/data/foo/gen/foo_parser.h"
        --output=${FOOC_PARSER_OUT}
        --report=state
        --report-file="${CMAKE_BINARY_DIR}/fooc_bison.log"
        "${CMAKE_SOURCE_DIR}/data/foo/foo_parser.y"
        COMMAND ${FLEX_EXECUTABLE}
        --outfile=${FOOC_SCANNER_OUT}
        "${CMAKE_SOURCE_DIR}/data/foo/foo_scanner.l"
        DEPENDS "${CMAKE_SOURCE_DIR}/data/foo/foo_parser.y"
        "${CMAKE_SOURCE_DIR}/data/foo/foo_scanner.l")

add_library(foo ${FOOC_CC})
add_dependencies(foo fooc_parser)

# ---------------------------------------------------------------------------
# Compiler
# ---------------------------------------------------------------------------
add_executable(fooc data/foo/foo.cc)
target_link_libraries(fooc foo gflags Threads::Threads)


