static void print_md5(FILE *stream, unsigned char digest[16]) {
 int i;

 for (i = 0; i < 16; ++i)
    fprintf(stream, "%02x", digest[i]);
}
