static int adev_open_input_stream(struct audio_hw_device *dev,
 audio_io_handle_t handle,
 audio_devices_t devices,
 struct audio_config *config,
 struct audio_stream_in **stream_in,
 audio_input_flags_t flags,
 const char *address,
 audio_source_t source)
{
 struct a2dp_audio_device *a2dp_dev = (struct a2dp_audio_device *)dev;
 struct a2dp_stream_in *in;
 int ret;
    UNUSED(address);
    UNUSED(config);
    UNUSED(devices);
    UNUSED(flags);
    UNUSED(handle);
    UNUSED(source);

    FNLOG();

    in = (struct a2dp_stream_in *)calloc(1, sizeof(struct a2dp_stream_in));

 if (!in)
 return -ENOMEM;

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

  
    a2dp_stream_common_init(&in->common);

 *stream_in = &in->stream;
    a2dp_dev->input = in;

    a2dp_open_ctrl_path(&in->common);
 if (in->common.ctrl_fd == AUDIO_SKT_DISCONNECTED)
 {
        ERROR("ctrl socket failed to connect (%s)", strerror(errno));
        ret = -1;
 goto err_open;
 }

 if (a2dp_read_audio_config(&in->common) < 0) {
        ERROR("a2dp_read_audio_config failed (%s)", strerror(errno));
        ret = -1;
 goto err_open;
 }

    DEBUG("success");
 return 0;

err_open:
    free(in);
 *stream_in = NULL;
    a2dp_dev->input = NULL;
    ERROR("failed");
 return ret;
}
