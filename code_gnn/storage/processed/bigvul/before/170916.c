void OMXNodeInstance::signalEvent(OMX_EVENTTYPE event, OMX_U32 arg1, OMX_U32 arg2) {
    mOwner->OnEvent(mNodeID, event, arg1, arg2, NULL);
}
