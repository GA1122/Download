static status_t GetMimeTypeForVideoCoding(
        OMX_VIDEO_CODINGTYPE codingType, AString *mime) {
 for (size_t i = 0;
         i < sizeof(kVideoCodingMapEntry) / sizeof(kVideoCodingMapEntry[0]);
 ++i) {
 if (codingType == kVideoCodingMapEntry[i].mVideoCodingType) {
 *mime = kVideoCodingMapEntry[i].mMime;
 return OK;
 }
 }

    mime->clear();

 return ERROR_UNSUPPORTED;
}
