static void create_EXIF_internal(ExifElement_t* elements, int exifTagCount, int gpsTagCount, int hasDateTimeTag, char* Buffer)
{
 unsigned short NumEntries;
 int DataWriteIndex;
 int DirIndex;
 int DirExifLink = 0;

#ifdef SUPERDEBUG
    ALOGE("create_EXIF %d exif elements, %d gps elements", exifTagCount, gpsTagCount);
#endif

 MotorolaOrder = 0;

    memcpy(Buffer+2, "Exif\0\0II",8);
 Put16u(Buffer+10, 0x2a);

 DataWriteIndex = 16;
 Put32u(Buffer+12, DataWriteIndex-8);  

 {
 DirIndex = DataWriteIndex;
 NumEntries = 1 + exifTagCount;  
 if (gpsTagCount) {
 ++NumEntries;  
 }
 if (!hasDateTimeTag) {
 ++NumEntries;
 }
 DataWriteIndex += 2 + NumEntries*12 + 4;

 Put16u(Buffer+DirIndex, NumEntries);  
 DirIndex += 2;

 if (!hasDateTimeTag) {
 char* dateTime = NULL;
 char dateBuf[20];
 if (ImageInfo.numDateTimeTags) {
                dateTime = ImageInfo.DateTime;
 } else {
 FileTimeAsString(dateBuf);
                dateTime = dateBuf;
 }
            writeExifTagAndData(TAG_DATETIME,
                                FMT_STRING,
 20,
 (long)(char*)dateBuf,
                                FALSE,
 Buffer,
 &DirIndex,
 &DataWriteIndex);

 }
 if (exifTagCount > 0) {
 int i;
 for (i = 0; i < exifTagCount + gpsTagCount; i++) {
 if (elements[i].GpsTag) {
 continue;
 }
 const TagTable_t* entry = TagToTagTableEntry(elements[i].Tag);
 if (entry == NULL) {
 continue;
 }
#ifdef SUPERDEBUG
                ALOGE("create_EXIF saving tag %x value \"%s\"",elements[i].Tag, elements[i].Value);
#endif
                writeExifTagAndData(elements[i].Tag,
                                    entry->Format,
                                    entry->DataLength,
 (long)elements[i].Value,
                                    TRUE,
 Buffer,
 &DirIndex,
 &DataWriteIndex);
 }

 if (gpsTagCount) {
                writeExifTagAndData(TAG_GPSINFO,
                                    FMT_ULONG,
 1,
 DataWriteIndex-8,
                                    FALSE,
 Buffer,
 &DirIndex,
 &DataWriteIndex);
 }

 int exifDirPtr = DataWriteIndex-8;
 if (gpsTagCount) {
                exifDirPtr += 2 + gpsTagCount*12 + 4;
 }
 DirExifLink = DirIndex;
            writeExifTagAndData(TAG_EXIF_OFFSET,
                                FMT_ULONG,
 1,
                                exifDirPtr,
                                FALSE,
 Buffer,
 &DirIndex,
 &DataWriteIndex);
 }

 Put32u(Buffer+DirIndex, 0);
        printf("Ending Exif section DirIndex = %d DataWriteIndex %d", DirIndex, DataWriteIndex);
 }


 if (gpsTagCount) {
 DirIndex = DataWriteIndex;
        printf("Starting GPS section DirIndex = %d", DirIndex);
 NumEntries = gpsTagCount;
 DataWriteIndex += 2 + NumEntries*12 + 4;

 Put16u(Buffer+DirIndex, NumEntries);  
 DirIndex += 2;
 {
 int i;
 for (i = 0; i < exifTagCount + gpsTagCount; i++) {
 if (!elements[i].GpsTag) {
 continue;
 }
 const TagTable_t* entry = GpsTagToTagTableEntry(elements[i].Tag);
 if (entry == NULL) {
 continue;
 }
#ifdef SUPERDEBUG
                ALOGE("create_EXIF saving GPS tag %x value \"%s\"",elements[i].Tag, elements[i].Value);
#endif
                writeExifTagAndData(elements[i].Tag,
                                    entry->Format,
                                    entry->DataLength,
 (long)elements[i].Value,
                                    TRUE,
 Buffer,
 &DirIndex,
 &DataWriteIndex);
 }
 }

 Put32u(Buffer+DirIndex, 0);
        printf("Ending GPS section DirIndex = %d DataWriteIndex %d", DirIndex, DataWriteIndex);
 }

 {
 Put32u(Buffer+DirExifLink+8, DataWriteIndex-8);

        printf("Starting Thumbnail section DirIndex = %d", DirIndex);
 DirIndex = DataWriteIndex;
 NumEntries = 2;
 DataWriteIndex += 2 + NumEntries*12 + 4;

 Put16u(Buffer+DirIndex, NumEntries);  
 DirIndex += 2;
 {
            writeExifTagAndData(TAG_THUMBNAIL_OFFSET,
                                FMT_ULONG,
 1,
 DataWriteIndex-8,
                                FALSE,
 Buffer,
 &DirIndex,
 &DataWriteIndex);
 }

 {
            writeExifTagAndData(TAG_THUMBNAIL_LENGTH,
                                FMT_ULONG,
 1,
 0,
                                FALSE,
 Buffer,
 &DirIndex,
 &DataWriteIndex);
 }

 Put32u(Buffer+DirIndex, 0);
        printf("Ending Thumbnail section DirIndex = %d DataWriteIndex %d", DirIndex, DataWriteIndex);
 }


 Buffer[0] = (unsigned char)(DataWriteIndex >> 8);
 Buffer[1] = (unsigned char)DataWriteIndex;

 RemoveSectionType(M_EXIF);

 {
 unsigned char * NewBuf = malloc(DataWriteIndex);
 if (NewBuf == NULL){
 ErrFatal("Could not allocate memory");
 }
        memcpy(NewBuf, Buffer, DataWriteIndex);

 CreateSection(M_EXIF, NewBuf, DataWriteIndex);

        process_EXIF(NewBuf, DataWriteIndex);
 }
}
