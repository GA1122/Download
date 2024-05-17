bool ACodec::IdleToLoadedState::onOMXEvent(
        OMX_EVENTTYPE event, OMX_U32 data1, OMX_U32 data2) {
 switch (event) {
 case OMX_EventCmdComplete:
 {
 if (data1 != (OMX_U32)OMX_CommandStateSet
 || data2 != (OMX_U32)OMX_StateLoaded) {
                ALOGE("Unexpected command completion in IdleToLoadedState: %s(%u) %s(%u)",
                        asString((OMX_COMMANDTYPE)data1), data1,
                        asString((OMX_STATETYPE)data2), data2);
                mCodec->signalError(OMX_ErrorUndefined, FAILED_TRANSACTION);
 return true;
 }

            mCodec->changeState(mCodec->mLoadedState);

 return true;
 }

 default:
 return BaseState::onOMXEvent(event, data1, data2);
 }
}
