readStringFromParcelInplace(Parcel &p, char *str, size_t maxLen) {
 size_t s16Len;
 const char16_t *s16;

    s16 = p.readString16Inplace(&s16Len);
 if (s16 == NULL) {
 return NO_MEMORY;
 }
 size_t strLen = strnlen16to8(s16, s16Len);
 if ((strLen + 1) > maxLen) {
 return NO_MEMORY;
 }
 if (strncpy16to8(str, s16, strLen) == NULL) {
 return NO_MEMORY;
 } else {
 return NO_ERROR;
 }
}
