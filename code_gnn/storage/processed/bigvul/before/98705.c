NPError WebPluginDelegatePepper::DeviceAudioDestroyContext(
    NPDeviceContextAudio* context) {
  if (!context || !audio_contexts_.Lookup(
      reinterpret_cast<intptr_t>(context->reserved))) {
    return NPERR_INVALID_PARAM;
  }
  audio_contexts_.Remove(reinterpret_cast<intptr_t>(context->reserved));
  memset(context, 0, sizeof(NPDeviceContextAudio));
  return NPERR_NO_ERROR;
}
