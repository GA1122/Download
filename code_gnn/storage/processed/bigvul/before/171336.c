status_t OMXCodec::freeBuffersOnPort(
        OMX_U32 portIndex, bool onlyThoseWeOwn) {
 Vector<BufferInfo> *buffers = &mPortBuffers[portIndex];

 status_t stickyErr = OK;

 for (size_t i = buffers->size(); i-- > 0;) {
 BufferInfo *info = &buffers->editItemAt(i);

 if (onlyThoseWeOwn && info->mStatus == OWNED_BY_COMPONENT) {
 continue;
 }

        CHECK(info->mStatus == OWNED_BY_US
 || info->mStatus == OWNED_BY_NATIVE_WINDOW);

        CODEC_LOGV("freeing buffer %u on port %u", info->mBuffer, portIndex);

 status_t err = freeBuffer(portIndex, i);

 if (err != OK) {
            stickyErr = err;
 }

 }

    CHECK(onlyThoseWeOwn || buffers->isEmpty());

 return stickyErr;
}
