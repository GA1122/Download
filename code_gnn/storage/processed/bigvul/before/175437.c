int edid_get_max_channels(struct audio_device *adev)
{
 int max_channels = 2;
 struct mixer_ctl *ctl;
 (void)adev;

  
 return max_channels;
}
