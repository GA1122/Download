static int check_input_parameters(uint32_t sample_rate,
 audio_format_t format,
 int channel_count)
{
 if (format != AUDIO_FORMAT_PCM_16_BIT) return -EINVAL;

 if ((channel_count < 1) || (channel_count > 4)) return -EINVAL;

 switch (sample_rate) {
 case 8000:
 case 11025:
 case 12000:
 case 16000:
 case 22050:
 case 24000:
 case 32000:
 case 44100:
 case 48000:
 break;
 default:
 return -EINVAL;
 }

 return 0;
}
