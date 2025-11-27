find_package(Doxygen REQUIRED)

set(DOXYGEN_HTML_OUTPUT            ${PROJECT_BINARY_DIR}/docs/html)
set(DOXYGEN_JAVADOC_AUTOBRIEF      YES)
set(DOXYGEN_GENERATE_HTML          YES)
set(DOXYGEN_HAVE_DOT               YES)
set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
set(DOXYGEN_EXCLUDE_PATTERNS
  "${PROJECT_SOURCE_DIR}/external/*"
  "${PROJECT_SOURCE_DIR}/**config.hpp"
)

doxygen_add_docs(docs_target
  ${PROJECT_SOURCE_DIR}
)
