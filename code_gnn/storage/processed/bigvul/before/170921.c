status_t OMXNodeInstance::updateGraphicBufferInMeta_l(
        OMX_U32 portIndex, const sp<GraphicBuffer>& graphicBuffer,
        OMX::buffer_id buffer, OMX_BUFFERHEADERTYPE *header, bool updateCodecBuffer) {
 if (header == NULL) {
        ALOGE("b/25884056");
 return BAD_VALUE;
 }

 if (portIndex != kPortIndexInput && portIndex != kPortIndexOutput) {
 return BAD_VALUE;
 }

 BufferMeta *bufferMeta = (BufferMeta *)(header->pAppPrivate);
    sp<ABuffer> data = bufferMeta->getBuffer(
            header, !updateCodecBuffer  , false  );
    bufferMeta->setGraphicBuffer(graphicBuffer);
 MetadataBufferType metaType = mMetadataType[portIndex];
 if (metaType == kMetadataBufferTypeGrallocSource && !updateCodecBuffer) {
        metaType = kMetadataBufferTypeANWBuffer;
 }
 if (metaType == kMetadataBufferTypeGrallocSource
 && data->capacity() >= sizeof(VideoGrallocMetadata)) {
 VideoGrallocMetadata &metadata = *(VideoGrallocMetadata *)(data->data());
        metadata.eType = kMetadataBufferTypeGrallocSource;
        metadata.pHandle = graphicBuffer == NULL ? NULL : graphicBuffer->handle;
 } else if (metaType == kMetadataBufferTypeANWBuffer
 && data->capacity() >= sizeof(VideoNativeMetadata)) {
 VideoNativeMetadata &metadata = *(VideoNativeMetadata *)(data->data());
        metadata.eType = kMetadataBufferTypeANWBuffer;
        metadata.pBuffer = graphicBuffer == NULL ? NULL : graphicBuffer->getNativeBuffer();
        metadata.nFenceFd = -1;
 } else {
        CLOG_ERROR(updateGraphicBufferInMeta, BAD_VALUE, "%s:%u, %#x bad type (%d) or size (%u)",
            portString(portIndex), portIndex, buffer, mMetadataType[portIndex], header->nAllocLen);
 return BAD_VALUE;
 }

    CLOG_BUFFER(updateGraphicBufferInMeta, "%s:%u, %#x := %p",
            portString(portIndex), portIndex, buffer,
            graphicBuffer == NULL ? NULL : graphicBuffer->handle);
 return OK;
}
