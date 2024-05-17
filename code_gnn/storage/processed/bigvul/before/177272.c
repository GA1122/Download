bool ACodec::ExecutingToIdleState::onOMXEvent(
        OMX_EVENTTYPE event, OMX_U32 data1, OMX_U32 data2) {
 switch (event) {
 case OMX_EventCmdComplete:
 {
 if (data1 != (OMX_U32)OMX_CommandStateSet
 || data2 != (OMX_U32)OMX_StateIdle) {
                ALOGE("Unexpected command completion in ExecutingToIdleState: %s(%u) %s(%u)",
                        asString((OMX_COMMANDTYPE)data1), data1,
                        asString((OMX_STATETYPE)data2), data2);
                mCodec->signalError(OMX_ErrorUndefined, FAILED_TRANSACTION);
 return true;
 }

            mComponentNowIdle = true;

            changeStateIfWeOwnAllBuffers();

 return true;
 }

 case OMX_EventPortSettingsChanged:
 case OMX_EventBufferFlag:
 {
 return true;
 }

 default:
 return BaseState::onOMXEvent(event, data1, data2);
 }
}
