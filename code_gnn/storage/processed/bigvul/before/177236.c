ACodec::BaseState::PortMode ACodec::OutputPortSettingsChangedState::getPortMode(
        OMX_U32 portIndex) {
 if (portIndex == kPortIndexOutput) {
 return FREE_BUFFERS;
 }

    CHECK_EQ(portIndex, (OMX_U32)kPortIndexInput);

 return RESUBMIT_BUFFERS;
}
