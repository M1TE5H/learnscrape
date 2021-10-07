# learnscrape
Library for Machine Learning and Web Scraping

Copyright (c) 2021 by M1TE5H
Available under the MIT License


## Directory Structure
```
learnscrape/
├── CMakeLists.txt
├── README.MD
├── include
│	└── learnscrape
│		├── learn.h
│		└── scrape.h
├── lib
│	├── lib.md
│	└── nthslwned
│		├── CMakeLists.txt
│		├── include
│		│	└── nthslwned
│		│		└── nthslwned.h
│		├── src
│		│	└── nthslwned.cpp
│		└── test
│			├── nthslwnedTest.cpp
│			└── CMakeLists.txt
├── src
│	└── learnscrape.cpp
└── test
```

**Note:** The general acronym `nthslwned` corresponds to the nth static library with no external dependencies. Each `nthslwned` is nested in the `lib` directory following the same structure outlined above; and `nthslwned` must accordingly be featured in the main `CMakeLists.txt` file.


## Usage Guide
1. Make build directory: `mkdir build`
2. Change to build director: `cd build`
3. Run CMake: `cmake ..`
4. Execute learnscrape: `./learnscrape` 