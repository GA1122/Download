static void hexdump(const void *_data, size_t size) {
 const uint8_t *data = (const uint8_t *)_data;
 size_t offset = 0;
 while (offset < size) {
        printf("0x%04zx  ", offset);

 size_t n = size - offset;
 if (n > 16) {
            n = 16;
 }

 for (size_t i = 0; i < 16; ++i) {
 if (i == 8) {
                printf(" ");
 }

 if (offset + i < size) {
                printf("%02x ", data[offset + i]);
 } else {
                printf("   ");
 }
 }

        printf(" ");

 for (size_t i = 0; i < n; ++i) {
 if (isprint(data[offset + i])) {
                printf("%c", data[offset + i]);
 } else {
                printf(".");
 }
 }

        printf("\n");

        offset += 16;
 }
}
