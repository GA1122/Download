const char *get_snd_device_display_name(snd_device_t snd_device)
{
 const char *name = get_snd_device_name(snd_device);

 if (name == NULL)
        name = "SND DEVICE NOT FOUND";

 return name;
}
