int start_input_stream(struct stream_in *in)
{
  
 int ret = 0;
 bool recreate_resampler = false;
 struct audio_usecase *uc_info;
 struct audio_device *adev = in->dev;
 struct pcm_device_profile *pcm_profile;
 struct pcm_device *pcm_device;

    ALOGV("%s: enter: usecase(%d)", __func__, in->usecase);
    adev->active_input = in;
    pcm_profile = get_pcm_device(in->usecase_type, in->devices);
 if (pcm_profile == NULL) {
        ALOGE("%s: Could not find PCM device id for the usecase(%d)",
              __func__, in->usecase);
        ret = -EINVAL;
 goto error_config;
 }

 if (in->input_flags & AUDIO_INPUT_FLAG_FAST) {
        ALOGV("%s: change capture period size to low latency size %d",
              __func__, CAPTURE_PERIOD_SIZE_LOW_LATENCY);
        pcm_profile->config.period_size = CAPTURE_PERIOD_SIZE_LOW_LATENCY;
 }

    uc_info = (struct audio_usecase *)calloc(1, sizeof(struct audio_usecase));
    uc_info->id = in->usecase;
    uc_info->type = PCM_CAPTURE;
    uc_info->stream = (struct audio_stream *)in;
    uc_info->devices = in->devices;
    uc_info->in_snd_device = SND_DEVICE_NONE;
    uc_info->out_snd_device = SND_DEVICE_NONE;

    pcm_device = (struct pcm_device *)calloc(1, sizeof(struct pcm_device));
    pcm_device->pcm_profile = pcm_profile;
    list_init(&in->pcm_dev_list);
    list_add_tail(&in->pcm_dev_list, &pcm_device->stream_list_node);

    list_init(&uc_info->mixer_list);
    list_add_tail(&uc_info->mixer_list,
 &adev_get_mixer_for_card(adev,
                                       pcm_device->pcm_profile->card)->uc_list_node[uc_info->id]);

    list_add_tail(&adev->usecase_list, &uc_info->adev_list_node);

    select_devices(adev, in->usecase);

  
 if (in->config.rate != pcm_profile->config.rate) {
        recreate_resampler = true;
 }
    in->config = pcm_profile->config;

#ifdef PREPROCESSING_ENABLED
 if (in->aux_channels_changed) {
        in->config.channels = audio_channel_count_from_in_mask(in->aux_channels);
        recreate_resampler = true;
 }
#endif

 if (in->requested_rate != in->config.rate) {
        recreate_resampler = true;
 }

 if (recreate_resampler) {
 if (in->resampler) {
            release_resampler(in->resampler);
            in->resampler = NULL;
 }
        in->buf_provider.get_next_buffer = get_next_buffer;
        in->buf_provider.release_buffer = release_buffer;
        ret = create_resampler(in->config.rate,
                               in->requested_rate,
                               in->config.channels,
                               RESAMPLER_QUALITY_DEFAULT,
 &in->buf_provider,
 &in->resampler);
 }

  
    ALOGV("%s: Opening PCM device card_id(%d) device_id(%d), channels %d, smp rate %d format %d, \
          period_size %d", __func__, pcm_device->pcm_profile->card, pcm_device->pcm_profile->device,
          pcm_device->pcm_profile->config.channels,pcm_device->pcm_profile->config.rate,
          pcm_device->pcm_profile->config.format, pcm_device->pcm_profile->config.period_size);

 if (pcm_profile->type == PCM_HOTWORD_STREAMING) {
 if (!adev->sound_trigger_open_for_streaming) {
            ALOGE("%s: No handle to sound trigger HAL", __func__);
            ret = -EIO;
 goto error_open;
 }
        pcm_device->pcm = NULL;
        pcm_device->sound_trigger_handle =
                adev->sound_trigger_open_for_streaming();
 if (pcm_device->sound_trigger_handle <= 0) {
            ALOGE("%s: Failed to open DSP for streaming", __func__);
            ret = -EIO;
 goto error_open;
 }
        ALOGV("Opened DSP successfully");
 } else {
        pcm_device->sound_trigger_handle = 0;
        pcm_device->pcm = pcm_open(pcm_device->pcm_profile->card,
                                   pcm_device->pcm_profile->device,
                                   PCM_IN | PCM_MONOTONIC,
 &pcm_device->pcm_profile->config);
 if (pcm_device->pcm && !pcm_is_ready(pcm_device->pcm)) {
            ALOGE("%s: %s", __func__, pcm_get_error(pcm_device->pcm));
            pcm_close(pcm_device->pcm);
            pcm_device->pcm = NULL;
            ret = -EIO;
 goto error_open;
 }
 }

  
#ifdef PREPROCESSING_ENABLED
    in->proc_buf_frames = 0;
#endif
    in->proc_buf_size = 0;
    in->read_buf_size = 0;
    in->read_buf_frames = 0;

  
 if (in->resampler) {
        in->resampler->reset(in->resampler);
 }

    ALOGV("%s: exit", __func__);
 return ret;

error_open:
 if (in->resampler) {
        release_resampler(in->resampler);
        in->resampler = NULL;
 }
    stop_input_stream(in);

error_config:
    ALOGV("%s: exit: status(%d)", __func__, ret);
    adev->active_input = NULL;
 return ret;
}
