void Parcel::Blob::clear() {
    mFd = -1;
    mData = NULL;
    mSize = 0;
    mMutable = false;
}
