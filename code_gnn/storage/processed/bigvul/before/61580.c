static void mxf_handle_small_eubc(AVFormatContext *s)
{
    MXFContext *mxf = s->priv_data;

     
    AVStream *st = mxf_get_opatom_stream(mxf);
    if (!st)
        return;

     
    if (st->codecpar->codec_type != AVMEDIA_TYPE_AUDIO         ||
        !is_pcm(st->codecpar->codec_id)                        ||
        mxf->nb_index_tables != 1                              ||
        mxf->index_tables[0].nb_segments != 1                  ||
        mxf->index_tables[0].segments[0]->edit_unit_byte_count >= 32)
        return;

     
     
    mxf->edit_units_per_packet = 1920;
}
