cmake_minimum_required(VERSION 3.0.0)

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
  learn
  scrape
)

project(${LEARNSCRAPE_PROJECT_NAME})


# ================
# Project
# ================
add_executable(${LEARNSCRAPE_PROJECT_NAME} ${LEARNSCRAPE_SOURCE})

foreach(LIBRARY ${LEARNSCRAPE_LIBRARIES})
  add_subdirectory("${LEARNSCRAPE_LIBRARIES_DIRECTORY}/${LIBRARY}")
endforeach(LIBRARY)
target_link_libraries(${LEARNSCRAPE_PROJECT_NAME} ${LEARNSCRAPE_LIBRARIES})
