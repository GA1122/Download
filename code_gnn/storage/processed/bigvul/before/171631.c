static char * adev_get_parameters(const struct audio_hw_device *dev,
 const char *keys)
{
 struct str_parms *parms;
    UNUSED(dev);

    FNLOG();

    parms = str_parms_create_str(keys);

    str_parms_dump(parms);

    str_parms_destroy(parms);

 return strdup("");
}
