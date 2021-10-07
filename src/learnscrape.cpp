#include <iostream>

void PrintVersion(int VersionMajor, int VersionMinor, int VersionStage) {
  printf ("learnscrape version %d.%d,%d\n", VersionMajor, VersionMinor, VersionStage);
}

int main()
{
  PrintVersion(0,0,0);
}
