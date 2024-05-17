static int out_set_volume(struct audio_stream_out *stream, float left,
 float right)
{
 struct stream_out *out = (struct stream_out *)stream;
 struct audio_device *adev = out->dev;
 (void)right;

 if (out->usecase == USECASE_AUDIO_PLAYBACK_MULTI_CH) {
  
        out->muted = (left == 0.0f);
 return 0;
 }

 return -ENOSYS;
}
