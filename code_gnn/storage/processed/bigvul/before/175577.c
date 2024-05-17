static status_t getFrameSizeByOffset(const sp<DataSource> &source,
 off64_t offset, bool isWide, size_t *frameSize) {
 uint8_t header;
 ssize_t count = source->readAt(offset, &header, 1);
 if (count == 0) {
 return ERROR_END_OF_STREAM;
 } else if (count < 0) {
 return ERROR_IO;
 }

 unsigned FT = (header >> 3) & 0x0f;

 *frameSize = getFrameSize(isWide, FT);
 if (*frameSize == 0) {
 return ERROR_MALFORMED;
 }
 return OK;
}
