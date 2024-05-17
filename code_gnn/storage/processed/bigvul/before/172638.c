void create_EXIF(ExifElement_t* elements, int exifTagCount, int gpsTagCount, int hasDateTimeTag)
{
 const int EXIF_MAX_SIZE = 1024*64;
 char* Buffer = malloc(EXIF_MAX_SIZE);

 if (Buffer != NULL) {
        create_EXIF_internal(elements, exifTagCount, gpsTagCount, hasDateTimeTag, Buffer);
        free(Buffer);
 } else {
 ErrFatal("Could not allocate memory");
 }
}
