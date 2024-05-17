WebKit::WebSpeechRecognizer* RenderViewImpl::speechRecognizer() {
#if defined(ENABLE_INPUT_SPEECH)
  if (!speech_recognition_dispatcher_)
    speech_recognition_dispatcher_ = new SpeechRecognitionDispatcher(this);
#endif
  return speech_recognition_dispatcher_;
}
