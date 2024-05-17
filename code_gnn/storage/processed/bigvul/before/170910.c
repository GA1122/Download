int OMXNodeInstance::retrieveFenceFromMeta_l(
        OMX_BUFFERHEADERTYPE *header, OMX_U32 portIndex) {
    OMX_U32 metaSize = portIndex == kPortIndexInput ? header->nAllocLen : header->nFilledLen;
 int fenceFd = -1;
 if (mMetadataType[portIndex] == kMetadataBufferTypeANWBuffer
 && header->nAllocLen >= sizeof(VideoNativeMetadata)) {
 VideoNativeMetadata &nativeMeta = *(VideoNativeMetadata *)(header->pBuffer);
 if (nativeMeta.eType == kMetadataBufferTypeANWBuffer) {
            fenceFd = nativeMeta.nFenceFd;
            nativeMeta.nFenceFd = -1;
 }
 if (metaSize < sizeof(nativeMeta) && fenceFd >= 0) {
            CLOG_ERROR(foundFenceInEmptyMeta, BAD_VALUE, FULL_BUFFER(
                    NULL, header, nativeMeta.nFenceFd));
            fenceFd = -1;
 }
 }
 return fenceFd;
}
