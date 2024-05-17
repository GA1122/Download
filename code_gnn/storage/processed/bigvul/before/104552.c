bool ExtensionTtsPlatformImplWin::IsSpeaking() {
  if (speech_synthesizer_ && !paused_) {
    SPVOICESTATUS status;
    HRESULT result = speech_synthesizer_->GetStatus(&status, NULL);
    if (result == S_OK) {
      if (status.dwRunningState == 0 ||   
          status.dwRunningState == SPRS_IS_SPEAKING) {
        return true;
      }
    }
  }
   return false;
 }
