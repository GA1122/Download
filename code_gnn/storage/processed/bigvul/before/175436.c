static int do_out_standby_l(struct stream_out *out)
{
 struct audio_device *adev = out->dev;
 int status = 0;

    out->standby = true;
    out_close_pcm_devices(out);
    status = stop_output_stream(out);

 return status;
}
