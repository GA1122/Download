int TagNameToValue(const char* tagName)
{
 unsigned int i;
 for (i = 0; i < TAG_TABLE_SIZE; i++) {
 if (strcmp(TagTable[i].Desc, tagName) == 0) {
            printf("found tag %s val %d", TagTable[i].Desc, TagTable[i].Tag);
 return TagTable[i].Tag;
 }
 }
    printf("tag %s NOT FOUND", tagName);
 return -1;
}
