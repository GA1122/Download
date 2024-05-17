struct pcm_device_profile *get_pcm_device(usecase_type_t uc_type, audio_devices_t devices)
{
 int i;

    devices &= ~AUDIO_DEVICE_BIT_IN;

 if (!devices)
 return NULL;

 for (i = 0; pcm_devices[i] != NULL; i++) {
 if ((pcm_devices[i]->type == uc_type) &&
 (devices & pcm_devices[i]->devices) == devices)
 return pcm_devices[i];
 }

 return NULL;
}
