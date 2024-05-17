static int mov_write_dops_tag(AVIOContext *pb, MOVTrack *track)
{
    int64_t pos = avio_tell(pb);
    avio_wb32(pb, 0);
    ffio_wfourcc(pb, "dOps");
    avio_w8(pb, 0);  
    if (track->par->extradata_size < 19) {
        av_log(pb, AV_LOG_ERROR, "invalid extradata size\n");
        return AVERROR_INVALIDDATA;
    }
     
    avio_w8(pb, AV_RB8(track->par->extradata + 9));  
    avio_wb16(pb, AV_RL16(track->par->extradata + 10));  
    avio_wb32(pb, AV_RL32(track->par->extradata + 12));  
    avio_wb16(pb, AV_RL16(track->par->extradata + 16));  
     
    avio_write(pb, track->par->extradata + 18, track->par->extradata_size - 18);

    return update_size(pb, pos);
}
