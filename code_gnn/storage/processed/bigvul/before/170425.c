void Parcel::Blob::clear() {
    mMapped = false;
    mData = NULL;
    mSize = 0;
}
