#include <iostream>
using namespace std;

int main() {
    int szam = 0;
    int val = 10;

    __asm {
        mov eax, val; Move the value of 'val' into the eax register
        add eax, szam; Add 'szam' to eax
        mov szam, eax; Move the result back to 'szam'
    }

    printf("%d", szam);

    return 0;
}
