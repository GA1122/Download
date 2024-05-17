static int add_remove_audio_effect(const struct audio_stream *stream,
 effect_handle_t effect,
 bool enable)
{
 struct stream_in *in = (struct stream_in *)stream;
 struct audio_device *adev = in->dev;
 int status = 0;
 effect_descriptor_t desc;
#ifdef PREPROCESSING_ENABLED
 int i;
#endif
    status = (*effect)->get_descriptor(effect, &desc);
 if (status != 0)
 return status;

    ALOGI("add_remove_audio_effect(), effect type: %08x, enable: %d ", desc.type.timeLow, enable);

    pthread_mutex_lock(&adev->lock_inputs);
    lock_input_stream(in);
    pthread_mutex_lock(&in->dev->lock);
#ifndef PREPROCESSING_ENABLED
 if ((in->source == AUDIO_SOURCE_VOICE_COMMUNICATION) &&
            in->enable_aec != enable &&
 (memcmp(&desc.type, FX_IID_AEC, sizeof(effect_uuid_t)) == 0)) {
        in->enable_aec = enable;
 if (!in->standby)
            select_devices(in->dev, in->usecase);
 }
#else
 if (enable) {
 if (in->num_preprocessors >= MAX_PREPROCESSORS) {
            status = -ENOSYS;
 goto exit;
 }
        in->preprocessors[in->num_preprocessors].effect_itfe = effect;
        in->num_preprocessors ++;
  
        in_update_aux_channels(in, effect); 
        in->aux_channels_changed = true;
 } else {
  
 if (in->num_preprocessors <= 0) {
            status = -ENOSYS;
 goto exit;
 }
        status = -EINVAL;
 for (i = 0; i < in->num_preprocessors && status != 0; i++) {
 if ( in->preprocessors[i].effect_itfe == effect ) {
                ALOGV("add_remove_audio_effect found fx at index %d", i);
                free(in->preprocessors[i].channel_configs);
                in->num_preprocessors--;
                memcpy(in->preprocessors + i,
                       in->preprocessors + i + 1,
 (in->num_preprocessors - i) * sizeof(in->preprocessors[0]));
                memset(in->preprocessors + in->num_preprocessors,
 0,
 sizeof(in->preprocessors[0]));
                status = 0;
 }
 }
 if (status != 0)
 goto exit;
        in->aux_channels_changed = false;
        ALOGV("%s: enable(%d), in->aux_channels_changed(%d)",
              __func__, enable, in->aux_channels_changed);
 }
    ALOGI("%s:  num_preprocessors = %d", __func__, in->num_preprocessors);

exit:
#endif
    ALOGW_IF(status != 0, "add_remove_audio_effect() error %d", status);
    pthread_mutex_unlock(&in->dev->lock);
    pthread_mutex_unlock(&in->lock);
    pthread_mutex_unlock(&adev->lock_inputs);
 return status;
}
