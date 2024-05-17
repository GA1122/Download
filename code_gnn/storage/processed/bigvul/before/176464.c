static int streamReader(GifFileType* fileType, GifByteType* out, int size) {
 Stream* stream = (Stream*) fileType->UserData;
 return (int) stream->read(out, size);
}
