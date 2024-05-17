BaseAudioContext* BaseAudioContext::Create(
    Document& document,
    const AudioContextOptions* context_options,
    ExceptionState& exception_state) {
  return AudioContext::Create(document, context_options, exception_state);
}
