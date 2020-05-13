#include "rnalib.h"

int main(){
    RNALIB *test = new RNALIB();
    test->stringToFasta("ABBGBABBAB", "TEST");
    return EXIT_SUCCESS;
}
