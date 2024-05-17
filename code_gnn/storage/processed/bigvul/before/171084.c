ID3::~ID3() {
 if (mData) {
        free(mData);
        mData = NULL;
 }
}
