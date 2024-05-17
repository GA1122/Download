static void mxf_compute_edit_units_per_packet(MXFContext *mxf, AVStream *st)
{
    MXFTrack *track = st->priv_data;
    MXFIndexTable *t;

    if (!track)
        return;
    track->edit_units_per_packet = 1;
    if (track->wrapping != ClipWrapped)
        return;

    t = mxf_find_index_table(mxf, track->index_sid);

     
    if (st->codecpar->codec_type != AVMEDIA_TYPE_AUDIO         ||
        !is_pcm(st->codecpar->codec_id)                        ||
        !t                                                     ||
        t->nb_segments != 1                                    ||
        t->segments[0]->edit_unit_byte_count >= 32)
        return;

     
     
    track->edit_units_per_packet = FFMAX(1, track->edit_rate.num / track->edit_rate.den / 25);
}
