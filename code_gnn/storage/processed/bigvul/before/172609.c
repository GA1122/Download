     void CopyFromOMX(const OMX_BUFFERHEADERTYPE *header) {
 if (!mIsBackup) {
 return;
 }

        sp<ABuffer> codec = getBuffer(header, false  , true  );

        memcpy((OMX_U8 *)mMem->pointer() + header->nOffset, codec->data(), codec->size());
 }
