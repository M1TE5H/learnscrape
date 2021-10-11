#include <iostream>

/* DESIGN PATTERNS */
// 1. Singletons
// 2. Facades
// 3. Bridges
// 4. Strategies
// 5. Observers


class generalFeature {
private:
	char* variableName;
	char* standardUnit;	
	int dataType;
	double scalingMetric;
public:
};


class dataEntry {
private:

public:
};


class trainingSet {
private:
	generalFeature* featureList;
	dataEntry** inputArray;
	dataEntry* outputArray;
public:
};





void PrintVersion(int VersionMajor, int VersionMinor, int VersionStage) {
  printf ("learnscrape version %d.%d,%d\n", VersionMajor, VersionMinor, VersionStage);
}

int main(int argc, char **argv) {
  PrintVersion(0,0,0);
}
