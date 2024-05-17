void Parcel::Blob::init(bool mapped, void* data, size_t size) {
    mMapped = mapped;
    mData = data;
    mSize = size;
}
