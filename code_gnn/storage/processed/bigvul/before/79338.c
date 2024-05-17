static int mov_write_dfla_tag(AVIOContext *pb, MOVTrack *track)
{
    int64_t pos = avio_tell(pb);
    avio_wb32(pb, 0);
    ffio_wfourcc(pb, "dfLa");
    avio_w8(pb, 0);  
    avio_wb24(pb, 0);  

     
    if (track->par->extradata_size != FLAC_STREAMINFO_SIZE)
        return AVERROR_INVALIDDATA;

     
    avio_w8(pb, 1 << 7 | FLAC_METADATA_TYPE_STREAMINFO);  
    avio_wb24(pb, track->par->extradata_size);  
    avio_write(pb, track->par->extradata, track->par->extradata_size);  

    return update_size(pb, pos);
}
