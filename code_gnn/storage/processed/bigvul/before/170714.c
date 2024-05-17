OMX_ERRORTYPE OMXNodeInstance::OnEmptyBufferDone(
        OMX_IN OMX_HANDLETYPE  ,
        OMX_IN OMX_PTR pAppData,
        OMX_IN OMX_BUFFERHEADERTYPE* pBuffer) {
 OMXNodeInstance *instance = static_cast<OMXNodeInstance *>(pAppData);
 if (instance->mDying) {
 return OMX_ErrorNone;
 }
 return instance->owner()->OnEmptyBufferDone(instance->nodeID(),
            instance->findBufferID(pBuffer), pBuffer);
}
