static int mov_write_clap_tag(AVIOContext *pb, MOVTrack *track)
{
    avio_wb32(pb, 40);
    ffio_wfourcc(pb, "clap");
    avio_wb32(pb, track->par->width);  
    avio_wb32(pb, 1);  
    avio_wb32(pb, track->height);  
    avio_wb32(pb, 1);  
    avio_wb32(pb, 0);  
    avio_wb32(pb, 1);  
    avio_wb32(pb, 0);  
    avio_wb32(pb, 1);  
    return 40;
}
