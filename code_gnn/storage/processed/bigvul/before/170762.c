status_t OMXNodeInstance::updateGraphicBufferInMeta(
        OMX_U32 portIndex, const sp<GraphicBuffer>& graphicBuffer,
        OMX::buffer_id buffer) {
 Mutex::Autolock autoLock(mLock);

    OMX_BUFFERHEADERTYPE *header = findBufferHeader(buffer);
 VideoDecoderOutputMetaData *metadata =
 (VideoDecoderOutputMetaData *)(header->pBuffer);
 BufferMeta *bufferMeta = (BufferMeta *)(header->pAppPrivate);
    bufferMeta->setGraphicBuffer(graphicBuffer);
    metadata->eType = kMetadataBufferTypeGrallocSource;
    metadata->pHandle = graphicBuffer->handle;
    CLOG_BUFFER(updateGraphicBufferInMeta, "%s:%u, %#x := %p",
            portString(portIndex), portIndex, buffer, graphicBuffer->handle);
 return OK;
}
