int start_output_stream(struct stream_out *out)
{
 int ret = 0;
 struct audio_device *adev = out->dev;

    ALOGV("%s: enter: usecase(%d: %s) devices(%#x) channels(%d)",
          __func__, out->usecase, use_case_table[out->usecase], out->devices, out->config.channels);

    enable_output_path_l(out);

    ret = out_open_pcm_devices(out);
 if (ret != 0)
 goto error_open;
    ALOGV("%s: exit", __func__);
 return 0;
error_open:
    stop_output_stream(out);
 return ret;
}
