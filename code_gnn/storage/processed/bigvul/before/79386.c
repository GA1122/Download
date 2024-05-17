static int mov_write_prft_tag(AVIOContext *pb, MOVMuxContext *mov, int tracks)
{
    int64_t pos = avio_tell(pb), pts_us, ntp_ts;
    MOVTrack *first_track;

     
    if (tracks > 0)
        return 0;
    first_track = &(mov->tracks[0]);

    if (!first_track->entry) {
        av_log(mov->fc, AV_LOG_WARNING, "Unable to write PRFT, no entries in the track\n");
        return 0;
    }

    if (first_track->cluster[0].pts == AV_NOPTS_VALUE) {
        av_log(mov->fc, AV_LOG_WARNING, "Unable to write PRFT, first PTS is invalid\n");
        return 0;
    }

    if (mov->write_prft == MOV_PRFT_SRC_WALLCLOCK) {
        ntp_ts = ff_get_formatted_ntp_time(ff_ntp_time());
    } else if (mov->write_prft == MOV_PRFT_SRC_PTS) {
        pts_us = av_rescale_q(first_track->cluster[0].pts,
                              first_track->st->time_base, AV_TIME_BASE_Q);
        ntp_ts = ff_get_formatted_ntp_time(pts_us + NTP_OFFSET_US);
    } else {
        av_log(mov->fc, AV_LOG_WARNING, "Unsupported PRFT box configuration: %d\n",
               mov->write_prft);
        return 0;
    }

    avio_wb32(pb, 0);                            
    ffio_wfourcc(pb, "prft");                    
    avio_w8(pb, 1);                              
    avio_wb24(pb, 0);                            
    avio_wb32(pb, first_track->track_id);        
    avio_wb64(pb, ntp_ts);                       
    avio_wb64(pb, first_track->cluster[0].pts);  
    return update_size(pb, pos);
}
