static uint32_t in_get_aux_channels(struct stream_in *in)
{
 if (in->num_preprocessors == 0)
 return 0;

  
 if (!(in->devices & AUDIO_DEVICE_IN_BUILTIN_MIC & ~AUDIO_DEVICE_BIT_IN))
 return 0;

 return AUDIO_CHANNEL_INDEX_MASK_4;
}
