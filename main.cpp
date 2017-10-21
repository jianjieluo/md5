#include <iostream>
#include "md5.h"

#define LEN 7

int main() {
    // basic usage
    // string input;
    // cin >> input;
    // MD5 md5(input);
    // cout << md5.getDigest() << endl;

    string input[LEN] = {
        "",
        "a",
        "abc",
        "message digest",
        "abcdefghijklmnopqrstuvwxyz",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
        "12345678901234567890123456789012345678901234567890123456789012345678901234567890"};

    string expect[LEN] = {
        "d41d8cd98f00b204e9800998ecf8427e", "0cc175b9c0f1b6a831c399e269772661",
        "900150983cd24fb0d6963f7d28e17f72", "f96b697d7cb7938d525a2f31aaf161d0",
        "c3fcd3d76192e4007dfb496cca67e13b", "d174ab98d277d9f5a5611c2c9f419d9f",
        "57edf4a22be3c955ac49da2e2107b67a"};

    for (int i = 0; i < LEN; ++i) {
        MD5 md5(input[i]);
        cout << "--------------------------------" << endl;
        cout << "Test " << i << ":" << endl;
        cout << "Original Message: " << input[i] << endl;
        cout << "Expected Result:  " << expect[i] << endl;
        auto digest = md5.getDigest();
        cout << "Calculate Result: " << digest << endl;
        auto flag = (digest == expect[i]) ? true : false;
        if (flag) {
            cout << "PASS" << endl;
        } else {
            cout << "FAILED" << endl;
        }
    }

    return 0;
}

/*
MD5 ("") = d41d8cd98f00b204e9800998ecf8427e
MD5 ("a") = 0cc175b9c0f1b6a831c399e269772661
MD5 ("abc") = 900150983cd24fb0d6963f7d28e17f72
MD5 ("message digest") = f96b697d7cb7938d525a2f31aaf161d0
MD5 ("abcdefghijklmnopqrstuvwxyz") = c3fcd3d76192e4007dfb496cca67e13b
MD5 ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789") =
d174ab98d277d9f5a5611c2c9f419d9f
*/