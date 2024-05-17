OMX_ERRORTYPE SimpleSoftOMXComponent::getParameter(
        OMX_INDEXTYPE index, OMX_PTR params) {
 Mutex::Autolock autoLock(mLock);
 return internalGetParameter(index, params);
}
