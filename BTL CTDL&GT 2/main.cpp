#include "main.h"
//#include "restaurant.cpp"

int main(int argc, char* argv[]) {
    string fileName = "testFirst.txt";
    simulate(fileName);

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
}
