void ID3::removeUnsynchronization() {
 for (size_t i = 0; i + 1 < mSize; ++i) {
 if (mData[i] == 0xff && mData[i + 1] == 0x00) {
            memmove(&mData[i + 1], &mData[i + 2], mSize - i - 2);
 --mSize;
 }
 }
}
