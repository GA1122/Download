status_t GraphicBuffer::flatten(void*& buffer, size_t& size, int*& fds, size_t& count) const {
 size_t sizeNeeded = GraphicBuffer::getFlattenedSize();
 if (size < sizeNeeded) return NO_MEMORY;

 size_t fdCountNeeded = GraphicBuffer::getFdCount();
 if (count < fdCountNeeded) return NO_MEMORY;

 int32_t* buf = static_cast<int32_t*>(buffer);
    buf[0] = 'GBFR';
    buf[1] = width;
    buf[2] = height;
    buf[3] = stride;
    buf[4] = format;
    buf[5] = usage;
    buf[6] = static_cast<int32_t>(mId >> 32);
    buf[7] = static_cast<int32_t>(mId & 0xFFFFFFFFull);
    buf[8] = 0;
    buf[9] = 0;

 if (handle) {
        buf[8] = handle->numFds;
        buf[9] = handle->numInts;
 native_handle_t const* const h = handle;
        memcpy(fds,     h->data,             h->numFds*sizeof(int));
        memcpy(&buf[10], h->data + h->numFds, h->numInts*sizeof(int));
 }

    buffer = reinterpret_cast<void*>(static_cast<int*>(buffer) + sizeNeeded);
    size -= sizeNeeded;
 if (handle) {
        fds += handle->numFds;
        count -= handle->numFds;
 }

 return NO_ERROR;
}
