static bool LegacySniffMPEG4(
 const sp<DataSource> &source, String8 *mimeType, float *confidence) {
 uint8_t header[8];

 ssize_t n = source->readAt(4, header, sizeof(header));
 if (n < (ssize_t)sizeof(header)) {
 return false;
 }

 if (!memcmp(header, "ftyp3gp", 7) || !memcmp(header, "ftypmp42", 8)
 || !memcmp(header, "ftyp3gr6", 8) || !memcmp(header, "ftyp3gs6", 8)
 || !memcmp(header, "ftyp3ge6", 8) || !memcmp(header, "ftyp3gg6", 8)
 || !memcmp(header, "ftypisom", 8) || !memcmp(header, "ftypM4V ", 8)
 || !memcmp(header, "ftypM4A ", 8) || !memcmp(header, "ftypf4v ", 8)
 || !memcmp(header, "ftypkddi", 8) || !memcmp(header, "ftypM4VP", 8)) {
 *mimeType = MEDIA_MIMETYPE_CONTAINER_MPEG4;
 *confidence = 0.4;

 return true;
 }

 return false;
}
