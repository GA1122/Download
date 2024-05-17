const char *get_snd_device_name(snd_device_t snd_device)
{
 const char *name = NULL;

 if (snd_device >= SND_DEVICE_MIN && snd_device < SND_DEVICE_MAX)
        name = device_table[snd_device];

    ALOGE_IF(name == NULL, "%s: invalid snd device %d", __func__, snd_device);

 return name;
}
