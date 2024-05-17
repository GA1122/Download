 static int dv_extract_audio_info(DVDemuxContext* c, uint8_t* frame)
 {
     const uint8_t* as_pack;
     int freq, stype, smpls, quant, i, ach;
 
     as_pack = dv_extract_pack(frame, dv_audio_source);
     if (!as_pack || !c->sys) {     
         c->ach = 0;
         return 0;
     }
 
     smpls =  as_pack[1] & 0x3f;        
     freq  = (as_pack[4] >> 3) & 0x07;  
      stype = (as_pack[3] & 0x1f);       
      quant =  as_pack[4] & 0x07;        
  
//     if (stype > 3) {
//         av_log(c->fctx, AV_LOG_ERROR, "stype %d is invalid\n", stype);
//         c->ach = 0;
//         return 0;
//     }
// 
       
      ach = ((int[4]){  1,  0,  2,  4})[stype];
      if (ach == 1 && quant && freq == 2)
            if (!c->ast[i])
                break;
            avpriv_set_pts_info(c->ast[i], 64, 1, 30000);
            c->ast[i]->codec->codec_type = AVMEDIA_TYPE_AUDIO;
            c->ast[i]->codec->codec_id   = CODEC_ID_PCM_S16LE;
 
            av_init_packet(&c->audio_pkt[i]);
            c->audio_pkt[i].size         = 0;
            c->audio_pkt[i].data         = c->audio_buf[i];
            c->audio_pkt[i].stream_index = c->ast[i]->index;
            c->audio_pkt[i].flags       |= AV_PKT_FLAG_KEY;
        }