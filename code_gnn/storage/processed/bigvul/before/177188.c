static status_t GetVideoCodingTypeFromMime(
 const char *mime, OMX_VIDEO_CODINGTYPE *codingType) {
 for (size_t i = 0;
         i < sizeof(kVideoCodingMapEntry) / sizeof(kVideoCodingMapEntry[0]);
 ++i) {
 if (!strcasecmp(mime, kVideoCodingMapEntry[i].mMime)) {
 *codingType = kVideoCodingMapEntry[i].mVideoCodingType;
 return OK;
 }
 }

 *codingType = OMX_VIDEO_CodingUnused;

 return ERROR_UNSUPPORTED;
}
