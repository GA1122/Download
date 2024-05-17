OMX_ERRORTYPE OMXNodeInstance::OnFillBufferDone(
        OMX_IN OMX_HANDLETYPE  ,
        OMX_IN OMX_PTR pAppData,
        OMX_IN OMX_BUFFERHEADERTYPE* pBuffer) {
 if (pAppData == NULL) {
        ALOGE("b/25884056");
 return OMX_ErrorBadParameter;
 }
 OMXNodeInstance *instance = static_cast<OMXNodeInstance *>(pAppData);
 if (instance->mDying) {
 return OMX_ErrorNone;
 }
 int fenceFd = instance->retrieveFenceFromMeta_l(pBuffer, kPortIndexOutput);
 return instance->owner()->OnFillBufferDone(instance->nodeID(),
            instance->findBufferID(pBuffer), pBuffer, fenceFd);
}
