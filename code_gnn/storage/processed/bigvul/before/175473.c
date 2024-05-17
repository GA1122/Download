static bool is_supported_format(audio_format_t format)
{
 if (format == AUDIO_FORMAT_MP3 ||
 ((format & AUDIO_FORMAT_MAIN_MASK) == AUDIO_FORMAT_AAC))
 return true;

 return false;
}
