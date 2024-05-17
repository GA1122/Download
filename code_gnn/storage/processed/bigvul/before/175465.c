static int in_remove_audio_effect(const struct audio_stream *stream,
 effect_handle_t effect)
{
    ALOGV("%s: effect %p", __func__, effect);
 return add_remove_audio_effect(stream, effect, false  );
}
