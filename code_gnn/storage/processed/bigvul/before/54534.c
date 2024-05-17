static int mov_read_mvhd(MOVContext *c, AVIOContext *pb, MOVAtom atom)
{
    time_t creation_time;
    int version = avio_r8(pb);  
    avio_rb24(pb);  

    if (version == 1) {
        creation_time = avio_rb64(pb);
        avio_rb64(pb);
    } else {
        creation_time = avio_rb32(pb);
        avio_rb32(pb);  
    }
    mov_metadata_creation_time(&c->fc->metadata, creation_time);
    c->time_scale = avio_rb32(pb);  

    av_dlog(c->fc, "time scale = %i\n", c->time_scale);

    c->duration = (version == 1) ? avio_rb64(pb) : avio_rb32(pb);  
    if (c->time_scale > 0)
        c->fc->duration = av_rescale(c->duration, AV_TIME_BASE, c->time_scale);
    avio_rb32(pb);  

    avio_rb16(pb);  

    avio_skip(pb, 10);  

    avio_skip(pb, 36);  

    avio_rb32(pb);  
    avio_rb32(pb);  
    avio_rb32(pb);  
    avio_rb32(pb);  
    avio_rb32(pb);  
    avio_rb32(pb);  
    avio_rb32(pb);  
    return 0;
}
