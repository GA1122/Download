status_t Camera2Client::commandEnableFocusMoveMsgL(bool enable) {
 SharedParameters::Lock l(mParameters);
    l.mParameters.enableFocusMoveMessages = enable;

 return OK;
}
