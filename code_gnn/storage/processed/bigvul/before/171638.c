static int adev_set_parameters(struct audio_hw_device *dev, const char *kvpairs)
{
 struct a2dp_audio_device *a2dp_dev = (struct a2dp_audio_device *)dev;
 struct a2dp_stream_out *out = a2dp_dev->output;
 int retval = 0;

 if (out == NULL)
 return retval;

    INFO("state %d", out->common.state);

    retval = out->stream.common.set_parameters((struct audio_stream *)out, kvpairs);

 return retval;
}
