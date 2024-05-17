void BaseAudioContext::RejectPendingDecodeAudioDataResolvers() {
  for (auto& resolver : decode_audio_resolvers_)
    resolver->Reject(DOMException::Create(kInvalidStateError,
                                          "Audio context is going away"));
  decode_audio_resolvers_.clear();
}