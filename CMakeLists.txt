cmake_minimum_required(VERSION 3.14..3.18)

set(LEARNSCRAPE_PROJECT_NAME
  learnscrape
)

set(LEARNSCRAPE_SOURCE_DIRECTORY
  src
)

set(LEARNSCRAPE_SOURCE
  ${LEARNSCRAPE_SOURCE_DIRECTORY}/learnscrape.cpp
)

set(LEARNSCRAPE_LIBRARIES_DIRECTORY
  libs
)

set(LEARNSCRAPE_LIBRARIES
)

project(${LEARNSCRAPE_PROJECT_NAME}
  VERSION
    0.0.0
  DESCRIPTION
    "Machine Learning and Web Scraping"
  LANGUAGES
    CXX
)


# ================
# Project
# ================
add_executable(${LEARNSCRAPE_PROJECT_NAME} ${LEARNSCRAPE_SOURCE})

foreach(LIBRARY ${LEARNSCRAPE_LIBRARIES})
  add_subdirectory("${LEARNSCRAPE_LIBRARIES_DIRECTORY}/${LIBRARY}")
endforeach(LIBRARY)
target_link_libraries(${LEARNSCRAPE_PROJECT_NAME} ${LEARNSCRAPE_LIBRARIES})
