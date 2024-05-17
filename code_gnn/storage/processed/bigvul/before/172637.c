static const TagTable_t* TagToTagTableEntry(unsigned short tag)
{
 unsigned int i;
 for (i = 0; i < TAG_TABLE_SIZE; i++) {
 if (TagTable[i].Tag == tag) {
            printf("found tag %d", tag);
 int format = TagTable[i].Format;
 if (format == 0) {
                printf("tag %s format not defined ***** YOU MUST ADD THE FORMAT TO THE TagTable in exif.c!!!!", TagTable[i].Desc);
 return NULL;
 }
 return &TagTable[i];
 }
 }
    printf("tag %d NOT FOUND", tag);
 return NULL;
}
