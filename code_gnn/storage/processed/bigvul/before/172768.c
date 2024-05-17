status_t MPEG4Extractor::parseQTMetaKey(off64_t offset, size_t size) {
 if (size < 8) {
 return ERROR_MALFORMED;
 }

 uint32_t count;
 if (!mDataSource->getUInt32(offset + 4, &count)) {
 return ERROR_MALFORMED;
 }

 if (mMetaKeyMap.size() > 0) {
        ALOGW("'keys' atom seen again, discarding existing entries");
        mMetaKeyMap.clear();
 }

 off64_t keyOffset = offset + 8;
 off64_t stopOffset = offset + size;
 for (size_t i = 1; i <= count; i++) {
 if (keyOffset + 8 > stopOffset) {
 return ERROR_MALFORMED;
 }

 uint32_t keySize;
 if (!mDataSource->getUInt32(keyOffset, &keySize)
 || keySize < 8
 || keyOffset + keySize > stopOffset) {
 return ERROR_MALFORMED;
 }

 uint32_t type;
 if (!mDataSource->getUInt32(keyOffset + 4, &type)
 || type != FOURCC('m', 'd', 't', 'a')) {
 return ERROR_MALFORMED;
 }

        keySize -= 8;
        keyOffset += 8;

        sp<ABuffer> keyData = new ABuffer(keySize);
 if (keyData->data() == NULL) {
 return ERROR_MALFORMED;
 }
 if (mDataSource->readAt(
                keyOffset, keyData->data(), keySize) < (ssize_t) keySize) {
 return ERROR_MALFORMED;
 }

 AString key((const char *)keyData->data(), keySize);
        mMetaKeyMap.add(i, key);

        keyOffset += keySize;
 }
 return OK;
}
