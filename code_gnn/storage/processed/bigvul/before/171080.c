 virtual ssize_t readAt(off64_t offset, void *data, size_t size) {
 off64_t available = (offset >= (off64_t)mSize) ? 0ll : mSize - offset;

 size_t copy = (available > (off64_t)size) ? size : available;
        memcpy(data, mData + offset, copy);

 return copy;
 }
