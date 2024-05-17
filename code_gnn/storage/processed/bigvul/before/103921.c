WebKit::WebSpeechInputController* RenderView::speechInputController(
    WebKit::WebSpeechInputListener* listener) {
  if (!speech_input_dispatcher_)
    speech_input_dispatcher_ = new SpeechInputDispatcher(this, listener);
  return speech_input_dispatcher_;
}
