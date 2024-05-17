enum AVCodecID ff_mov_get_lpcm_codec_id(int bps, int flags)
{
     
    return ff_get_pcm_codec_id(bps, flags & 1, flags & 2, flags & 4 ? -1 : 0);
}
