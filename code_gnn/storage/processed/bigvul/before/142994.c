  mojom::blink::AudioContextManagerPtr& GetAudioContextManagerPtrFor(
      AudioContext* audio_context) {
    return audio_context->audio_context_manager_;
  }
