status_t OMXNodeInstance::emptyGraphicBuffer(
        OMX_BUFFERHEADERTYPE *header, const sp<GraphicBuffer> &graphicBuffer,
        OMX_U32 flags, OMX_TICKS timestamp, int fenceFd) {
 if (header == NULL) {
        ALOGE("b/25884056");
 return BAD_VALUE;
 }

 Mutex::Autolock autoLock(mLock);
    OMX::buffer_id buffer = findBufferID(header);
 status_t err = updateGraphicBufferInMeta_l(
            kPortIndexInput, graphicBuffer, buffer, header,
 true  );
 if (err != OK) {
        CLOG_ERROR(emptyGraphicBuffer, err, FULL_BUFFER(
 (intptr_t)header->pBuffer, header, fenceFd));
 return err;
 }

    header->nOffset = 0;
 if (graphicBuffer == NULL) {
        header->nFilledLen = 0;
 } else if (mMetadataType[kPortIndexInput] == kMetadataBufferTypeGrallocSource) {
        header->nFilledLen = sizeof(VideoGrallocMetadata);
 } else {
        header->nFilledLen = sizeof(VideoNativeMetadata);
 }
 return emptyBuffer_l(header, flags, timestamp, (intptr_t)header->pBuffer, fenceFd);
}
