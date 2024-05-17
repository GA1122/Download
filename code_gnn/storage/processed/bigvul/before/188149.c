      void CopyFromOMX(const OMX_BUFFERHEADERTYPE *header) {
        if (!mIsBackup) {
//         if (!mCopyFromOmx) {
              return;
          }
  
         sp<ABuffer> codec = getBuffer(header, false  , true  );
 
         memcpy((OMX_U8 *)mMem->pointer() + header->nOffset, codec->data(), codec->size());
 
      }