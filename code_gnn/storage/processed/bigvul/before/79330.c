static int mov_write_avid_tag(AVIOContext *pb, MOVTrack *track)
{
    int i;
    int interlaced;
    int cid;
    int display_width = track->par->width;

    if (track->vos_data && track->vos_len > 0x29) {
        if (ff_dnxhd_parse_header_prefix(track->vos_data) != 0) {
             
            interlaced = (track->vos_data[5] & 2);
            cid = AV_RB32(track->vos_data + 0x28);
        } else {
            av_log(NULL, AV_LOG_WARNING, "Could not locate DNxHD bit stream in vos_data\n");
            return 0;
        }
    } else {
        av_log(NULL, AV_LOG_WARNING, "Could not locate DNxHD bit stream, vos_data too small\n");
        return 0;
    }

    avio_wb32(pb, 24);  
    ffio_wfourcc(pb, "ACLR");
    ffio_wfourcc(pb, "ACLR");
    ffio_wfourcc(pb, "0001");
    if (track->par->color_range == AVCOL_RANGE_MPEG ||  
        track->par->color_range == AVCOL_RANGE_UNSPECIFIED) {
        avio_wb32(pb, 1);  
    } else {  
        avio_wb32(pb, 2);  
    }
    avio_wb32(pb, 0);  

    if (track->tag == MKTAG('A','V','d','h')) {
        avio_wb32(pb, 32);
        ffio_wfourcc(pb, "ADHR");
        ffio_wfourcc(pb, "0001");
        avio_wb32(pb, cid);
        avio_wb32(pb, 0);  
        avio_wb32(pb, 1);  
        avio_wb32(pb, 0);  
        avio_wb32(pb, 0);  
        return 0;
    }

    avio_wb32(pb, 24);  
    ffio_wfourcc(pb, "APRG");
    ffio_wfourcc(pb, "APRG");
    ffio_wfourcc(pb, "0001");
    avio_wb32(pb, 1);  
    avio_wb32(pb, 0);  

    avio_wb32(pb, 120);  
    ffio_wfourcc(pb, "ARES");
    ffio_wfourcc(pb, "ARES");
    ffio_wfourcc(pb, "0001");
    avio_wb32(pb, cid);  
    if (   track->par->sample_aspect_ratio.num > 0
        && track->par->sample_aspect_ratio.den > 0)
        display_width = display_width * track->par->sample_aspect_ratio.num / track->par->sample_aspect_ratio.den;
    avio_wb32(pb, display_width);
     
    if (interlaced) {
        avio_wb32(pb, track->par->height / 2);
        avio_wb32(pb, 2);  
        avio_wb32(pb, 0);  
        avio_wb32(pb, 4);  
    } else {
        avio_wb32(pb, track->par->height);
        avio_wb32(pb, 1);  
        avio_wb32(pb, 0);  
        if (track->par->height == 1080)
            avio_wb32(pb, 5);  
        else
            avio_wb32(pb, 6);  
    }
     
    for (i = 0; i < 10; i++)
        avio_wb64(pb, 0);

    return 0;
}
