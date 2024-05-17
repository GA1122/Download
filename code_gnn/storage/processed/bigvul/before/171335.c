status_t OMXCodec::freeBuffer(OMX_U32 portIndex, size_t bufIndex) {
 Vector<BufferInfo> *buffers = &mPortBuffers[portIndex];

 BufferInfo *info = &buffers->editItemAt(bufIndex);

 status_t err = mOMX->freeBuffer(mNode, portIndex, info->mBuffer);

 if (err == OK && info->mMediaBuffer != NULL) {
        CHECK_EQ(portIndex, (OMX_U32)kPortIndexOutput);
        info->mMediaBuffer->setObserver(NULL);

        CHECK_EQ(info->mMediaBuffer->refcount(), 0);

        sp<GraphicBuffer> graphicBuffer = info->mMediaBuffer->graphicBuffer();
 if (info->mStatus == OWNED_BY_US && graphicBuffer != 0) {
            err = cancelBufferToNativeWindow(info);
 }

        info->mMediaBuffer->release();
        info->mMediaBuffer = NULL;
 }

 if (err == OK) {
        buffers->removeAt(bufIndex);
 }

 return err;
}
