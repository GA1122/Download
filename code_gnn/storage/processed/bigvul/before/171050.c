void log_hex(const char *who, const uint8_t *buffer, int length) {
 char out[80];
 int source = 0;
 int dest = 0;
 int dest_len = sizeof(out);
 int per_line = 0;

 do {
        dest += sprintf(out, "%8.8s [%8.8x] ", who, source);
 for(; source < length && dest_len - dest > 3 && per_line < BYTES_PER_LINE; source++,
        per_line ++) {
            out[dest++] = HEX_HIGH(buffer[source]);
            out[dest++] = HEX_LOW(buffer[source]);
            out[dest++] = ' ';
 }
 if (dest < dest_len && (per_line == BYTES_PER_LINE || source >= length)) {
            out[dest++] = 0;
            per_line = 0;
            dest = 0;
            RLOGD("%s\n", out);
 }
 } while(source < length && dest < dest_len);
}
