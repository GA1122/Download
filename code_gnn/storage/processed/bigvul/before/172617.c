status_t OMXNodeInstance::emptyGraphicBuffer(
        OMX_BUFFERHEADERTYPE *header, const sp<GraphicBuffer> &graphicBuffer,
        OMX_U32 flags, OMX_TICKS timestamp, int fenceFd) {
 Mutex::Autolock autoLock(mLock);
    OMX::buffer_id buffer = findBufferID(header);
 status_t err = updateGraphicBufferInMeta_l(kPortIndexInput, graphicBuffer, buffer, header);
 if (err != OK) {
        CLOG_ERROR(emptyGraphicBuffer, err, FULL_BUFFER(
 (intptr_t)header->pBuffer, header, fenceFd));
 return err;
 }

    header->nOffset = 0;
    header->nFilledLen = graphicBuffer == NULL ? 0 : header->nAllocLen;
 return emptyBuffer_l(header, flags, timestamp, (intptr_t)header->pBuffer, fenceFd);
}
