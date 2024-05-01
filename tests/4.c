int foo();

int main() {
    int a = 3;
    {
        int e = 7;
    }
    if (a) {
        int f = 14;
    }
    else {
        int g = 42;
        if (a) {
            int h = 100;
        }
        else {
            int i = 200;
        }
    }
    int b = 5, c = 6;
    int d = a + b;
    foo();
    return 0;
}


// rbp - 16 a  rsp + 0
// rbp - 12 b  rsp + 4
// rbp - 8  c
// rbp - 4  d
// rbp - 0  ret
