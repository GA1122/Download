status_t ATSParser::PSISection::append(const void *data, size_t size) {
 if (mBuffer == NULL || mBuffer->size() + size > mBuffer->capacity()) {
 size_t newCapacity =
 (mBuffer == NULL) ? size : mBuffer->capacity() + size;

        newCapacity = (newCapacity + 1023) & ~1023;

        sp<ABuffer> newBuffer = new ABuffer(newCapacity);

 if (mBuffer != NULL) {
            memcpy(newBuffer->data(), mBuffer->data(), mBuffer->size());
            newBuffer->setRange(0, mBuffer->size());
 } else {
            newBuffer->setRange(0, 0);
 }

        mBuffer = newBuffer;
 }

    memcpy(mBuffer->data() + mBuffer->size(), data, size);
    mBuffer->setRange(0, mBuffer->size() + size);

 return OK;
}
