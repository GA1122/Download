status_t MPEG4Extractor::parseQTMetaVal(
 int32_t keyId, off64_t offset, size_t size) {
 ssize_t index = mMetaKeyMap.indexOfKey(keyId);
 if (index < 0) {
 return ERROR_MALFORMED;
 }

 if (size <= 16) {
 return ERROR_MALFORMED;
 }
 uint32_t dataSize;
 if (!mDataSource->getUInt32(offset, &dataSize)
 || dataSize > size || dataSize <= 16) {
 return ERROR_MALFORMED;
 }
 uint32_t atomFourCC;
 if (!mDataSource->getUInt32(offset + 4, &atomFourCC)
 || atomFourCC != FOURCC('d', 'a', 't', 'a')) {
 return ERROR_MALFORMED;
 }
 uint32_t dataType;
 if (!mDataSource->getUInt32(offset + 8, &dataType)
 || ((dataType & 0xff000000) != 0)) {
 return ERROR_MALFORMED;
 }

    dataSize -= 16;
    offset += 16;

 if (dataType == 23 && dataSize >= 4) {
 uint32_t val;
 if (!mDataSource->getUInt32(offset, &val)) {
 return ERROR_MALFORMED;
 }
 if (!strcasecmp(mMetaKeyMap[index].c_str(), "com.android.capture.fps")) {
            mFileMetaData->setFloat(kKeyCaptureFramerate, *(float *)&val);
 }
 } else if (dataType == 67 && dataSize >= 4) {
 uint32_t val;
 if (!mDataSource->getUInt32(offset, &val)) {
 return ERROR_MALFORMED;
 }
 if (!strcasecmp(mMetaKeyMap[index].c_str(), "com.android.video.temporal_layers_count")) {
            mFileMetaData->setInt32(kKeyTemporalLayerCount, val);
 }
 } else {
        ALOGV("ignoring key: type %d, size %d", dataType, dataSize);
 }

 return OK;
}
