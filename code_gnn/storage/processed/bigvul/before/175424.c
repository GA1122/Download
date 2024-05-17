static int adev_open_output_stream(struct audio_hw_device *dev,
 audio_io_handle_t handle,
 audio_devices_t devices,
 audio_output_flags_t flags,
 struct audio_config *config,
 struct audio_stream_out **stream_out,
 const char *address __unused)
{
 struct audio_device *adev = (struct audio_device *)dev;
 struct stream_out *out;
 int i, ret;
 struct pcm_device_profile *pcm_profile;

    ALOGV("%s: enter: sample_rate(%d) channel_mask(%#x) devices(%#x) flags(%#x)",
          __func__, config->sample_rate, config->channel_mask, devices, flags);
 *stream_out = NULL;
    out = (struct stream_out *)calloc(1, sizeof(struct stream_out));

 if (devices == AUDIO_DEVICE_NONE)
        devices = AUDIO_DEVICE_OUT_SPEAKER;

    out->flags = flags;
    out->devices = devices;
    out->dev = adev;
    out->format = config->format;
    out->sample_rate = config->sample_rate;
    out->channel_mask = AUDIO_CHANNEL_OUT_STEREO;
    out->supported_channel_masks[0] = AUDIO_CHANNEL_OUT_STEREO;
    out->handle = handle;

    pcm_profile = get_pcm_device(PCM_PLAYBACK, devices);
 if (pcm_profile == NULL) {
        ret = -EINVAL;
 goto error_open;
 }
    out->config = pcm_profile->config;

  
 if (out->flags & (AUDIO_OUTPUT_FLAG_DEEP_BUFFER)) {
        out->usecase = USECASE_AUDIO_PLAYBACK_DEEP_BUFFER;
        out->config = pcm_config_deep_buffer;
        out->sample_rate = out->config.rate;
        ALOGV("%s: use AUDIO_PLAYBACK_DEEP_BUFFER",__func__);
 } else {
        out->usecase = USECASE_AUDIO_PLAYBACK;
        out->sample_rate = out->config.rate;
 }

 if (flags & AUDIO_OUTPUT_FLAG_PRIMARY) {
 if (adev->primary_output == NULL)
            adev->primary_output = out;
 else {
            ALOGE("%s: Primary output is already opened", __func__);
            ret = -EEXIST;
 goto error_open;
 }
 }

  
    pthread_mutex_lock(&adev->lock);
 if (get_usecase_from_id(adev, out->usecase) != NULL) {
        ALOGE("%s: Usecase (%d) is already present", __func__, out->usecase);
        pthread_mutex_unlock(&adev->lock);
        ret = -EEXIST;
 goto error_open;
 }
    pthread_mutex_unlock(&adev->lock);

    out->stream.common.get_sample_rate = out_get_sample_rate;
    out->stream.common.set_sample_rate = out_set_sample_rate;
    out->stream.common.get_buffer_size = out_get_buffer_size;
    out->stream.common.get_channels = out_get_channels;
    out->stream.common.get_format = out_get_format;
    out->stream.common.set_format = out_set_format;
    out->stream.common.standby = out_standby;
    out->stream.common.dump = out_dump;
    out->stream.common.set_parameters = out_set_parameters;
    out->stream.common.get_parameters = out_get_parameters;
    out->stream.common.add_audio_effect = out_add_audio_effect;
    out->stream.common.remove_audio_effect = out_remove_audio_effect;
    out->stream.get_latency = out_get_latency;
    out->stream.set_volume = out_set_volume;
    out->stream.write = out_write;
    out->stream.get_render_position = out_get_render_position;
    out->stream.get_next_write_timestamp = out_get_next_write_timestamp;
    out->stream.get_presentation_position = out_get_presentation_position;

    out->standby = 1;
  
  

    pthread_mutex_init(&out->lock, (const pthread_mutexattr_t *) NULL);
    pthread_mutex_init(&out->pre_lock, (const pthread_mutexattr_t *) NULL);
    pthread_cond_init(&out->cond, (const pthread_condattr_t *) NULL);

    config->format = out->stream.common.get_format(&out->stream.common);
    config->channel_mask = out->stream.common.get_channels(&out->stream.common);
    config->sample_rate = out->stream.common.get_sample_rate(&out->stream.common);

 *stream_out = &out->stream;
    ALOGV("%s: exit", __func__);
 return 0;

error_open:
    free(out);
 *stream_out = NULL;
    ALOGV("%s: exit: ret %d", __func__, ret);
 return ret;
}
