void SoftAVC::onPortFlushCompleted(OMX_U32 portIndex) {
 if (portIndex == kInputPortIndex) {
        mEOSStatus = INPUT_DATA_AVAILABLE;
 }
}
