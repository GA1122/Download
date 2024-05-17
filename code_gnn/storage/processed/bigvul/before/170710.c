 void CopyFromOMX(const OMX_BUFFERHEADERTYPE *header) {
 if (!mIsBackup) {
 return;
 }

        memcpy((OMX_U8 *)mMem->pointer() + header->nOffset,
                header->pBuffer + header->nOffset,
                header->nFilledLen);
 }
