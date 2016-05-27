#include <iostream>

using namespace std;

extern "C" int SampleAddInt(int i1, int i2);
extern "C" void SampleFunction1();

int main()
{
    cout << "HI\n";
    cout << SampleAddInt(1, 2) << "\n";
    SampleFunction1();
    cout << "The end!\n";

    return 0;
}
