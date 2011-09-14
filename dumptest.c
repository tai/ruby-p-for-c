/*BINFMTC:-Wall -O0 -g3 -I.
 *
 * Test for "pp.h".
 * Dumps data structure without prior knowledge in actual structure.
 */

#include "pp.h"

struct testdata {
    char *name;
    int a, b;
};

int
main(int argc, char **argv) {
    struct testdata data;
    data.name = "hogehoge";
    data.a = 123;
    data.b = 234;
    p(data);
    p(*(&data));
    p(*(&data), data.a);
    //p(main);
    return 0;
}
