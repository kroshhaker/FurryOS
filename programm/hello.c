void main() {
    const char *msg = "Hello from userland!\n";
    for (int i = 0; msg[i]; i++) {
        asm volatile("int $0x80" : : "a"(1), "b"(msg[i]));
    }

    asm volatile("int $0x80" : : "a"(0)); // exit(0)
}
