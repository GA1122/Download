void Parcel::Blob::release() {
 if (mFd != -1 && mData) {
 ::munmap(mData, mSize);
 }
    clear();
}
