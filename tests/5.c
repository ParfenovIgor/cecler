int boo(int aa, int bb, int cc) {

}

int foo(int a, int b, int c, int d) {
    boo(a, d, b);
}

int main() {
    foo(1, 2, 3, 4);
    return 0;
}
