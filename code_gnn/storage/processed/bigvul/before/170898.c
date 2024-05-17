    sp<ABuffer> getBuffer(const OMX_BUFFERHEADERTYPE *header, bool backup, bool limit) {
        sp<ABuffer> buf;
 if (backup && mMem != NULL) {
            buf = new ABuffer(mMem->pointer(), mMem->size());
 } else {
            buf = new ABuffer(header->pBuffer, header->nAllocLen);
 }
 if (limit) {
 if (header->nOffset + header->nFilledLen > header->nOffset
 && header->nOffset + header->nFilledLen <= header->nAllocLen) {
                buf->setRange(header->nOffset, header->nFilledLen);
 } else {
                buf->setRange(0, 0);
 }
 }
 return buf;
 }
