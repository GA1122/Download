status_t OMXNodeInstance::allocateBufferWithBackup(
        OMX_U32 portIndex, const sp<IMemory> &params,
        OMX::buffer_id *buffer) {
 Mutex::Autolock autoLock(mLock);

 BufferMeta *buffer_meta = new BufferMeta(params, true);

    OMX_BUFFERHEADERTYPE *header;

    OMX_ERRORTYPE err = OMX_AllocateBuffer(
            mHandle, &header, portIndex, buffer_meta, params->size());

 if (err != OMX_ErrorNone) {
        CLOG_ERROR(allocateBufferWithBackup, err,
                SIMPLE_BUFFER(portIndex, params->size(), params->pointer()));
 delete buffer_meta;
        buffer_meta = NULL;

 *buffer = 0;

 return StatusFromOMXError(err);
 }

    CHECK_EQ(header->pAppPrivate, buffer_meta);

 *buffer = makeBufferID(header);

    addActiveBuffer(portIndex, *buffer);

    sp<GraphicBufferSource> bufferSource(getGraphicBufferSource());
 if (bufferSource != NULL && portIndex == kPortIndexInput) {
        bufferSource->addCodecBuffer(header);
 }

    CLOG_BUFFER(allocateBufferWithBackup, NEW_BUFFER_FMT(*buffer, portIndex, "%zu@%p :> %p",
            params->size(), params->pointer(), header->pBuffer));

 return OK;
}
