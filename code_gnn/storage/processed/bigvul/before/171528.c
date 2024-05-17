OMX_ERRORTYPE SimpleSoftOMXComponent::getState(OMX_STATETYPE *state) {
 Mutex::Autolock autoLock(mLock);

 *state = mState;

 return OMX_ErrorNone;
}
