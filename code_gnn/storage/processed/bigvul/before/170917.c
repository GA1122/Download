status_t OMXNodeInstance::storeFenceInMeta_l(
        OMX_BUFFERHEADERTYPE *header, int fenceFd, OMX_U32 portIndex) {
    OMX_U32 metaSize = portIndex == kPortIndexInput ? header->nFilledLen : header->nAllocLen;
 if (mMetadataType[portIndex] == kMetadataBufferTypeANWBuffer
 && metaSize >= sizeof(VideoNativeMetadata)) {
 VideoNativeMetadata &nativeMeta = *(VideoNativeMetadata *)(header->pBuffer);
 if (nativeMeta.nFenceFd >= 0) {
            ALOGE("fence (%d) already exists in meta", nativeMeta.nFenceFd);
 if (fenceFd >= 0) {
 ::close(fenceFd);
 }
 return ALREADY_EXISTS;
 }
        nativeMeta.nFenceFd = fenceFd;
 } else if (fenceFd >= 0) {
        CLOG_BUFFER(storeFenceInMeta, "waiting for fence %d", fenceFd);
        sp<Fence> fence = new Fence(fenceFd);
 return fence->wait(IOMX::kFenceTimeoutMs);
 }
 return OK;
}
