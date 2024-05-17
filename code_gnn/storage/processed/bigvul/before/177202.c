status_t ACodec::allocateBuffersOnPort(OMX_U32 portIndex) {
    CHECK(portIndex == kPortIndexInput || portIndex == kPortIndexOutput);

    CHECK(mDealer[portIndex] == NULL);
    CHECK(mBuffers[portIndex].isEmpty());

 status_t err;
 if (mNativeWindow != NULL && portIndex == kPortIndexOutput) {
 if (storingMetadataInDecodedBuffers()) {
            err = allocateOutputMetadataBuffers();
 } else {
            err = allocateOutputBuffersFromNativeWindow();
 }
 } else {
        OMX_PARAM_PORTDEFINITIONTYPE def;
 InitOMXParams(&def);
        def.nPortIndex = portIndex;

        err = mOMX->getParameter(
                mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 if (err == OK) {
 MetadataBufferType type =
                portIndex == kPortIndexOutput ? mOutputMetadataType : mInputMetadataType;
 int32_t bufSize = def.nBufferSize;
 if (type == kMetadataBufferTypeGrallocSource) {
                bufSize = sizeof(VideoGrallocMetadata);
 } else if (type == kMetadataBufferTypeANWBuffer) {
                bufSize = sizeof(VideoNativeMetadata);
 }

 int32_t allottedSize = bufSize;
 if (portIndex == kPortIndexInput && type >= kMetadataBufferTypeGrallocSource) {
                bufSize = max(sizeof(VideoGrallocMetadata), sizeof(VideoNativeMetadata));
 } else if (portIndex == kPortIndexInput && type == kMetadataBufferTypeCameraSource) {
                bufSize = max(bufSize, (int32_t)sizeof(VideoNativeMetadata));
 }

            ALOGV("[%s] Allocating %u buffers of size %d/%d (from %u using %s) on %s port",
                    mComponentName.c_str(),
                    def.nBufferCountActual, bufSize, allottedSize, def.nBufferSize, asString(type),
                    portIndex == kPortIndexInput ? "input" : "output");

 size_t totalSize = def.nBufferCountActual * bufSize;
            mDealer[portIndex] = new MemoryDealer(totalSize, "ACodec");

 for (OMX_U32 i = 0; i < def.nBufferCountActual && err == OK; ++i) {
                sp<IMemory> mem = mDealer[portIndex]->allocate(bufSize);
 if (mem == NULL || mem->pointer() == NULL) {
 return NO_MEMORY;
 }

 BufferInfo info;
                info.mStatus = BufferInfo::OWNED_BY_US;
                info.mFenceFd = -1;
                info.mRenderInfo = NULL;

 uint32_t requiresAllocateBufferBit =
 (portIndex == kPortIndexInput)
 ? OMXCodec::kRequiresAllocateBufferOnInputPorts
 : OMXCodec::kRequiresAllocateBufferOnOutputPorts;

 if ((portIndex == kPortIndexInput && (mFlags & kFlagIsSecure))
 || (portIndex == kPortIndexOutput && usingMetadataOnEncoderOutput())) {
                    mem.clear();

 void *ptr;
                    err = mOMX->allocateBuffer(
                            mNode, portIndex, bufSize, &info.mBufferID,
 &ptr);

                    info.mData = new ABuffer(ptr, bufSize);
 } else if (mQuirks & requiresAllocateBufferBit) {
                    err = mOMX->allocateBufferWithBackup(
                            mNode, portIndex, mem, &info.mBufferID, allottedSize);
 } else {
                    err = mOMX->useBuffer(mNode, portIndex, mem, &info.mBufferID, allottedSize);
 }

 if (mem != NULL) {
                    info.mData = new ABuffer(mem->pointer(), bufSize);
 if (type == kMetadataBufferTypeANWBuffer) {
 ((VideoNativeMetadata *)mem->pointer())->nFenceFd = -1;
 }
 }

                mBuffers[portIndex].push(info);
 }
 }
 }

 if (err != OK) {
 return err;
 }

    sp<AMessage> notify = mNotify->dup();
    notify->setInt32("what", CodecBase::kWhatBuffersAllocated);

    notify->setInt32("portIndex", portIndex);

    sp<PortDescription> desc = new PortDescription;

 for (size_t i = 0; i < mBuffers[portIndex].size(); ++i) {
 const BufferInfo &info = mBuffers[portIndex][i];

        desc->addBuffer(info.mBufferID, info.mData);
 }

    notify->setObject("portDesc", desc);
    notify->post();

 return OK;
}
