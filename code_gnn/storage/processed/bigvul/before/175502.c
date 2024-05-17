static int set_voice_volume_l(struct audio_device *adev, float volume)
{
 int err = 0;
 (void)volume;

 if (adev->mode == AUDIO_MODE_IN_CALL) {
  
 }
 return err;
}
