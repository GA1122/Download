static int get_snd_codec_id(audio_format_t format)
{
 int id = 0;

 switch (format & AUDIO_FORMAT_MAIN_MASK) {
 default:
        ALOGE("%s: Unsupported audio format", __func__);
 }

 return id;
}
