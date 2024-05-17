static int adev_open_input_stream(struct audio_hw_device *dev,
 audio_io_handle_t handle __unused,
 audio_devices_t devices,
 struct audio_config *config,
 struct audio_stream_in **stream_in,
 audio_input_flags_t flags,
 const char *address __unused,
 audio_source_t source)
{
 struct audio_device *adev = (struct audio_device *)dev;
 struct stream_in *in;
 struct pcm_device_profile *pcm_profile;

    ALOGV("%s: enter", __func__);

 *stream_in = NULL;
 if (check_input_parameters(config->sample_rate, config->format,
                               audio_channel_count_from_in_mask(config->channel_mask)) != 0)
 return -EINVAL;

 usecase_type_t usecase_type = (source == AUDIO_SOURCE_HOTWORD) ?
                PCM_HOTWORD_STREAMING : PCM_CAPTURE;
    pcm_profile = get_pcm_device(usecase_type, devices);
 if (pcm_profile == NULL)
 return -EINVAL;

    in = (struct stream_in *)calloc(1, sizeof(struct stream_in));

    in->stream.common.get_sample_rate = in_get_sample_rate;
    in->stream.common.set_sample_rate = in_set_sample_rate;
    in->stream.common.get_buffer_size = in_get_buffer_size;
    in->stream.common.get_channels = in_get_channels;
    in->stream.common.get_format = in_get_format;
    in->stream.common.set_format = in_set_format;
    in->stream.common.standby = in_standby;
    in->stream.common.dump = in_dump;
    in->stream.common.set_parameters = in_set_parameters;
    in->stream.common.get_parameters = in_get_parameters;
    in->stream.common.add_audio_effect = in_add_audio_effect;
    in->stream.common.remove_audio_effect = in_remove_audio_effect;
    in->stream.set_gain = in_set_gain;
    in->stream.read = in_read;
    in->stream.get_input_frames_lost = in_get_input_frames_lost;

    in->devices = devices;
    in->source = source;
    in->dev = adev;
    in->standby = 1;
    in->main_channels = config->channel_mask;
    in->requested_rate = config->sample_rate;
 if (config->sample_rate != CAPTURE_DEFAULT_SAMPLING_RATE)
        flags = flags & ~AUDIO_INPUT_FLAG_FAST;
    in->input_flags = flags;
  
    in->config = pcm_profile->config;

  
 if (source == AUDIO_SOURCE_HOTWORD) {
        in->usecase = USECASE_AUDIO_CAPTURE_HOTWORD;
 } else {
        in->usecase = USECASE_AUDIO_CAPTURE;
 }
    in->usecase_type = usecase_type;

    pthread_mutex_init(&in->lock, (const pthread_mutexattr_t *) NULL);
    pthread_mutex_init(&in->pre_lock, (const pthread_mutexattr_t *) NULL);

 *stream_in = &in->stream;
    ALOGV("%s: exit", __func__);
 return 0;
}
