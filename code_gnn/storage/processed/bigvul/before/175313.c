static bool BetterSniffMPEG4(
 const sp<DataSource> &source, String8 *mimeType, float *confidence,
        sp<AMessage> *meta) {
 static const off64_t kMaxScanOffset = 128ll;

 off64_t offset = 0ll;
 bool foundGoodFileType = false;
 off64_t moovAtomEndOffset = -1ll;
 bool done = false;

 while (!done && offset < kMaxScanOffset) {
 uint32_t hdr[2];
 if (source->readAt(offset, hdr, 8) < 8) {
 return false;
 }

 uint64_t chunkSize = ntohl(hdr[0]);
 uint32_t chunkType = ntohl(hdr[1]);
 off64_t chunkDataOffset = offset + 8;

 if (chunkSize == 1) {
 if (source->readAt(offset + 8, &chunkSize, 8) < 8) {
 return false;
 }

            chunkSize = ntoh64(chunkSize);
            chunkDataOffset += 8;

 if (chunkSize < 16) {
 return false;
 }

 } else if (chunkSize < 8) {
 return false;
 }

 off64_t chunkDataSize = chunkSize - (chunkDataOffset - offset);
 if (chunkDataSize < 0) {
            ALOGE("b/23540914");
 return ERROR_MALFORMED;
 }

 char chunkstring[5];
 MakeFourCCString(chunkType, chunkstring);
        ALOGV("saw chunk type %s, size %" PRIu64 " @ %lld", chunkstring, chunkSize, (long long)offset);
 switch (chunkType) {
 case FOURCC('f', 't', 'y', 'p'):
 {
 if (chunkDataSize < 8) {
 return false;
 }

 uint32_t numCompatibleBrands = (chunkDataSize - 8) / 4;
 for (size_t i = 0; i < numCompatibleBrands + 2; ++i) {
 if (i == 1) {
 continue;
 }

 uint32_t brand;
 if (source->readAt(
                                chunkDataOffset + 4 * i, &brand, 4) < 4) {
 return false;
 }

                    brand = ntohl(brand);

 if (isCompatibleBrand(brand)) {
                        foundGoodFileType = true;
 break;
 }
 }

 if (!foundGoodFileType) {
 return false;
 }

 break;
 }

 case FOURCC('m', 'o', 'o', 'v'):
 {
                moovAtomEndOffset = offset + chunkSize;

                done = true;
 break;
 }

 default:
 break;
 }

        offset += chunkSize;
 }

 if (!foundGoodFileType) {
 return false;
 }

 *mimeType = MEDIA_MIMETYPE_CONTAINER_MPEG4;
 *confidence = 0.4f;

 if (moovAtomEndOffset >= 0) {
 *meta = new AMessage;
 (*meta)->setInt64("meta-data-size", moovAtomEndOffset);

        ALOGV("found metadata size: %lld", (long long)moovAtomEndOffset);
 }

 return true;
}
