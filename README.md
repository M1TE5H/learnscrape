# learnscrape
Materials Informatics Library for Machine Learning and Web Scraping

## Abstract
Multivariate Polynomial Regression in C/C++ with Complete Database Framework using Ruby HTTP Clients and Selenium.

## Permissions
Copyright (c) 2021 by Dr Mitesh Patel (M1TE5H)

This restricted version of the source code is made available for free under the MIT License. Contact the developer for the full version:
```bash
  npx mitesh
```

## Disclaimer
This source code comes without any warranties as to the accuracy and reliability of the data referenced, compiled, presented and/or extrapolated. The user is advised to conduct their own due diligence.

## Contents



## Directory Structure
Static Library
```
learnscrape/
├── CMakeLists.txt
├── README.MD
├── include
│   └── learnscrape
│   ├── learn.h
│   └── scrape.h
├── lib
│   ├── lib.md
│   └── nthslwned
│       ├── CMakeLists.txt
│       ├── include
│       │   └── nthslwned
│       │       └── nthslwned.h
│       ├── src
│       │   └── nthslwned.cpp
│       └── test
│           ├── CMakeLists.txt
│           └── nthslwnedTest.cpp
├── src
│   └── learnscrape.cpp
└── test
```

**Note:** The general acronym `nthslwned` corresponds to the nth static library with no external dependencies. Each `nthslwned` is nested in the `lib` directory following the same structure outlined above; and `nthslwned` must accordingly be featured in the main `CMakeLists.txt` file.

## External Dependencies

### C/C++
Linear Algebra:
- `llapack`
- `lblas`
- `larmadillo`

### Ruby
Standard Library:
- `open-uri`
- `net/http`
- `csv`

Gem Installation:
```bash
  gem install `pry`     	# Debugging Tools
  gem install `nokogiri`	# Parsing 
  gem install `httparty`	# HTTP Request
  gem install 'kimurai'
```
### MacOS Tools
```bash
  brew cask install google-chrome firefox
  brew cask install chromedriver
  brew install geckodriver
  brew install phantomjs
```

## Usage Guide
1. Make build directory: `mkdir build`
2. Change to build director: `cd build`
3. Run CMake: `cmake ..`
4. Execute learnscrape: `./learnscrape` 
