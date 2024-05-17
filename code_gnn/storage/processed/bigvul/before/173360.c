static size_t pad_size(size_t s) {
 if (s > (SIZE_T_MAX - 3)) {
        abort();
 }
 return PAD_SIZE_UNSAFE(s);
}
