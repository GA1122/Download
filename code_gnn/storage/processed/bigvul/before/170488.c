void Parcel::Blob::release() {
 if (mMapped && mData) {
 ::munmap(mData, mSize);
 }
    clear();
}
