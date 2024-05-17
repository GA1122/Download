WebKit::WebSpeechInputController* RenderViewImpl::speechInputController(
    WebKit::WebSpeechInputListener* listener) {
#if defined(ENABLE_INPUT_SPEECH)
  if (!input_tag_speech_dispatcher_)
    input_tag_speech_dispatcher_ =
        new InputTagSpeechDispatcher(this, listener);
#endif
  return input_tag_speech_dispatcher_;
}
