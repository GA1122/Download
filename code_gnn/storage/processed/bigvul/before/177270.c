bool ACodec::ExecutingState::onOMXEvent(
        OMX_EVENTTYPE event, OMX_U32 data1, OMX_U32 data2) {
 switch (event) {
 case OMX_EventPortSettingsChanged:
 {
            CHECK_EQ(data1, (OMX_U32)kPortIndexOutput);

 if (data2 == 0 || data2 == OMX_IndexParamPortDefinition) {
                mCodec->mMetadataBuffersToSubmit = 0;
                CHECK_EQ(mCodec->mOMX->sendCommand(
                            mCodec->mNode,
                            OMX_CommandPortDisable, kPortIndexOutput),
 (status_t)OK);

                mCodec->freeOutputBuffersNotOwnedByComponent();

                mCodec->changeState(mCodec->mOutputPortSettingsChangedState);
 } else if (data2 == OMX_IndexConfigCommonOutputCrop) {
                mCodec->mSentFormat = false;

 if (mCodec->mTunneled) {
                    sp<AMessage> dummy = new AMessage(kWhatOutputBufferDrained, mCodec);
                    mCodec->sendFormatChange(dummy);
 }
 } else {
                ALOGV("[%s] OMX_EventPortSettingsChanged 0x%08x",
                     mCodec->mComponentName.c_str(), data2);
 }

 return true;
 }

 case OMX_EventBufferFlag:
 {
 return true;
 }

 default:
 return BaseState::onOMXEvent(event, data1, data2);
 }
}
