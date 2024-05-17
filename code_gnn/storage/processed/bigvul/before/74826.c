static void mxf_compute_essence_containers(AVFormatContext *s)
{
    MXFContext *mxf = s->priv_data;
    int x;

    for (x = 0; x < mxf->partitions_count; x++) {
        MXFPartition *p = &mxf->partitions[x];
        MXFWrappingScheme wrapping;

        if (!p->body_sid)
            continue;        

         

        wrapping = (mxf->op == OPAtom) ? ClipWrapped : mxf_get_wrapping_by_body_sid(s, p->body_sid);

        if (wrapping == ClipWrapped) {
            p->essence_offset = p->first_essence_klv.next_klv - p->first_essence_klv.length;
            p->essence_length = p->first_essence_klv.length;
        } else {
            p->essence_offset = p->first_essence_klv.offset;

             
            if (x < mxf->partitions_count - 1)
                p->essence_length = mxf->partitions[x+1].this_partition - p->essence_offset;

            if (p->essence_length < 0) {
                 
                p->essence_length = 0;
                av_log(mxf->fc, AV_LOG_ERROR,
                       "partition %i: bad ThisPartition = %"PRIX64"\n",
                       x+1, mxf->partitions[x+1].this_partition);
            }
        }
    }
}
