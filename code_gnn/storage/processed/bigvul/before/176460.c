static bool isGif(void* header, int header_size) {
 return !memcmp(GIF_STAMP, header, GIF_STAMP_LEN)
 || !memcmp(GIF87_STAMP, header, GIF_STAMP_LEN)
 || !memcmp(GIF89_STAMP, header, GIF_STAMP_LEN);
}
