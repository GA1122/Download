NPError WebPluginDelegatePepper::DeviceAudioGetStateContext(
    NPDeviceContextAudio* context,
    int32 state,
    intptr_t* value) {
  if (state == NPExtensionsReservedStateSharedMemory) {
    if (!context)
      return NPERR_INVALID_PARAM;
    AudioDeviceContext* ctx = audio_contexts_.Lookup(
        reinterpret_cast<intptr_t>(context->reserved));
    if (!ctx)
      return NPERR_INVALID_PARAM;
    *value = reinterpret_cast<intptr_t>(ctx->shared_memory());
    return NPERR_NO_ERROR;
  } else if (state == NPExtensionsReservedStateSharedMemorySize) {
    if (!context)
      return NPERR_INVALID_PARAM;
    AudioDeviceContext* ctx = audio_contexts_.Lookup(
        reinterpret_cast<intptr_t>(context->reserved));
    if (!ctx)
      return NPERR_INVALID_PARAM;
    *value = static_cast<intptr_t>(ctx->shared_memory_size());
    return NPERR_NO_ERROR;
  } else if (state == NPExtensionsReservedStateSyncChannel) {
    if (!context)
      return NPERR_INVALID_PARAM;
    AudioDeviceContext* ctx = audio_contexts_.Lookup(
        reinterpret_cast<intptr_t>(context->reserved));
    if (!ctx)
      return NPERR_INVALID_PARAM;
    *value = reinterpret_cast<intptr_t>(ctx->socket());
    return NPERR_NO_ERROR;
  }
  return NPERR_GENERIC_ERROR;
}
