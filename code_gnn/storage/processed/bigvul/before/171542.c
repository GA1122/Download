OMX_ERRORTYPE SimpleSoftOMXComponent::setParameter(
        OMX_INDEXTYPE index, const OMX_PTR params) {
 Mutex::Autolock autoLock(mLock);

    CHECK(isSetParameterAllowed(index, params));

 return internalSetParameter(index, params);
}
