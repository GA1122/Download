bool ID3::parseV1(const sp<DataSource> &source) {
 const size_t V1_TAG_SIZE = 128;

 off64_t size;
 if (source->getSize(&size) != OK || size < (off64_t)V1_TAG_SIZE) {
 return false;
 }

    mData = (uint8_t *)malloc(V1_TAG_SIZE);
 if (source->readAt(size - V1_TAG_SIZE, mData, V1_TAG_SIZE)
 != (ssize_t)V1_TAG_SIZE) {
        free(mData);
        mData = NULL;

 return false;
 }

 if (memcmp("TAG", mData, 3)) {
        free(mData);
        mData = NULL;

 return false;
 }

    mSize = V1_TAG_SIZE;
    mFirstFrameOffset = 3;

 if (mData[V1_TAG_SIZE - 3] != 0) {
        mVersion = ID3_V1;
 } else {
        mVersion = ID3_V1_1;
 }

 return true;
}
