static int mov_check_timecode_track(AVFormatContext *s, AVTimecode *tc, int src_index, const char *tcstr)
{
    int ret;

     
    ret = av_timecode_init_from_string(tc, find_fps(s,  s->streams[src_index]), tcstr, s);
    return ret;
}
