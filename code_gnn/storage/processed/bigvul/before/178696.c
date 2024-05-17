 static int ape_read_header(AVFormatContext * s, AVFormatParameters * ap)
 {
     AVIOContext *pb = s->pb;
     APEContext *ape = s->priv_data;
     AVStream *st;
     uint32_t tag;
     int i;
     int total_blocks;
     int64_t pts;
 
      
     ape->junklength = 0;
 
     tag = avio_rl32(pb);
     if (tag != MKTAG('M', 'A', 'C', ' '))
         return -1;
 
     ape->fileversion = avio_rl16(pb);
 
     if (ape->fileversion < APE_MIN_VERSION || ape->fileversion > APE_MAX_VERSION) {
         av_log(s, AV_LOG_ERROR, "Unsupported file version - %d.%02d\n", ape->fileversion / 1000, (ape->fileversion % 1000) / 10);
         return -1;
     }
 
     if (ape->fileversion >= 3980) {
         ape->padding1             = avio_rl16(pb);
         ape->descriptorlength     = avio_rl32(pb);
         ape->headerlength         = avio_rl32(pb);
         ape->seektablelength      = avio_rl32(pb);
         ape->wavheaderlength      = avio_rl32(pb);
         ape->audiodatalength      = avio_rl32(pb);
         ape->audiodatalength_high = avio_rl32(pb);
         ape->wavtaillength        = avio_rl32(pb);
         avio_read(pb, ape->md5, 16);
 
          
         if (ape->descriptorlength > 52)
             avio_seek(pb, ape->descriptorlength - 52, SEEK_CUR);
 
          
         ape->compressiontype      = avio_rl16(pb);
         ape->formatflags          = avio_rl16(pb);
         ape->blocksperframe       = avio_rl32(pb);
         ape->finalframeblocks     = avio_rl32(pb);
         ape->totalframes          = avio_rl32(pb);
         ape->bps                  = avio_rl16(pb);
         ape->channels             = avio_rl16(pb);
         ape->samplerate           = avio_rl32(pb);
     } else {
         ape->descriptorlength = 0;
         ape->headerlength = 32;
 
         ape->compressiontype      = avio_rl16(pb);
         ape->formatflags          = avio_rl16(pb);
         ape->channels             = avio_rl16(pb);
         ape->samplerate           = avio_rl32(pb);
         ape->wavheaderlength      = avio_rl32(pb);
         ape->wavtaillength        = avio_rl32(pb);
         ape->totalframes          = avio_rl32(pb);
         ape->finalframeblocks     = avio_rl32(pb);
 
         if (ape->formatflags & MAC_FORMAT_FLAG_HAS_PEAK_LEVEL) {
             avio_seek(pb, 4, SEEK_CUR);  
             ape->headerlength += 4;
         }
 
         if (ape->formatflags & MAC_FORMAT_FLAG_HAS_SEEK_ELEMENTS) {
             ape->seektablelength = avio_rl32(pb);
             ape->headerlength += 4;
             ape->seektablelength *= sizeof(int32_t);
         } else
             ape->seektablelength = ape->totalframes * sizeof(int32_t);
 
         if (ape->formatflags & MAC_FORMAT_FLAG_8_BIT)
             ape->bps = 8;
         else if (ape->formatflags & MAC_FORMAT_FLAG_24_BIT)
             ape->bps = 24;
         else
             ape->bps = 16;
 
         if (ape->fileversion >= 3950)
             ape->blocksperframe = 73728 * 4;
         else if (ape->fileversion >= 3900 || (ape->fileversion >= 3800  && ape->compressiontype >= 4000))
             ape->blocksperframe = 73728;
         else
             ape->blocksperframe = 9216;
 
          
         if (!(ape->formatflags & MAC_FORMAT_FLAG_CREATE_WAV_HEADER))
              avio_seek(pb, ape->wavheaderlength, SEEK_CUR);
      }
  
//     if(!ape->totalframes){
//         av_log(s, AV_LOG_ERROR, "No frames in the file!\n");
//         return AVERROR(EINVAL);
//     }
      if(ape->totalframes > UINT_MAX / sizeof(APEFrame)){
          av_log(s, AV_LOG_ERROR, "Too many frames: %d\n", ape->totalframes);
          return -1;
     }
     ape->frames       = av_malloc(ape->totalframes * sizeof(APEFrame));
     if(!ape->frames)
         return AVERROR(ENOMEM);
     ape->firstframe   = ape->junklength + ape->descriptorlength + ape->headerlength + ape->seektablelength + ape->wavheaderlength;
     ape->currentframe = 0;
 
 
     ape->totalsamples = ape->finalframeblocks;
     if (ape->totalframes > 1)
         ape->totalsamples += ape->blocksperframe * (ape->totalframes - 1);
 
     if (ape->seektablelength > 0) {
         ape->seektable = av_malloc(ape->seektablelength);
         for (i = 0; i < ape->seektablelength / sizeof(uint32_t); i++)
             ape->seektable[i] = avio_rl32(pb);
     }
 
     ape->frames[0].pos     = ape->firstframe;
     ape->frames[0].nblocks = ape->blocksperframe;
     ape->frames[0].skip    = 0;
     for (i = 1; i < ape->totalframes; i++) {
         ape->frames[i].pos      = ape->seektable[i];  
         ape->frames[i].nblocks  = ape->blocksperframe;
         ape->frames[i - 1].size = ape->frames[i].pos - ape->frames[i - 1].pos;
         ape->frames[i].skip     = (ape->frames[i].pos - ape->frames[0].pos) & 3;
     }
     ape->frames[ape->totalframes - 1].size    = ape->finalframeblocks * 4;
     ape->frames[ape->totalframes - 1].nblocks = ape->finalframeblocks;
 
     for (i = 0; i < ape->totalframes; i++) {
         if(ape->frames[i].skip){
             ape->frames[i].pos  -= ape->frames[i].skip;
             ape->frames[i].size += ape->frames[i].skip;
         }
         ape->frames[i].size = (ape->frames[i].size + 3) & ~3;
     }
 
 
     ape_dumpinfo(s, ape);
 
      
     if (!url_is_streamed(pb)) {
         ff_ape_parse_tag(s);
         avio_seek(pb, 0, SEEK_SET);
     }
 
     av_log(s, AV_LOG_DEBUG, "Decoding file - v%d.%02d, compression level %d\n", ape->fileversion / 1000, (ape->fileversion % 1000) / 10, ape->compressiontype);
 
      
     st = av_new_stream(s, 0);
     if (!st)
         return -1;
 
     total_blocks = (ape->totalframes == 0) ? 0 : ((ape->totalframes - 1) * ape->blocksperframe) + ape->finalframeblocks;
 
     st->codec->codec_type      = AVMEDIA_TYPE_AUDIO;
     st->codec->codec_id        = CODEC_ID_APE;
     st->codec->codec_tag       = MKTAG('A', 'P', 'E', ' ');
     st->codec->channels        = ape->channels;
     st->codec->sample_rate     = ape->samplerate;
     st->codec->bits_per_coded_sample = ape->bps;
     st->codec->frame_size      = MAC_SUBFRAME_SIZE;
 
     st->nb_frames = ape->totalframes;
     st->start_time = 0;
     st->duration  = total_blocks / MAC_SUBFRAME_SIZE;
     av_set_pts_info(st, 64, MAC_SUBFRAME_SIZE, ape->samplerate);
 
     st->codec->extradata = av_malloc(APE_EXTRADATA_SIZE);
     st->codec->extradata_size = APE_EXTRADATA_SIZE;
     AV_WL16(st->codec->extradata + 0, ape->fileversion);
     AV_WL16(st->codec->extradata + 2, ape->compressiontype);
     AV_WL16(st->codec->extradata + 4, ape->formatflags);
 
     pts = 0;
     for (i = 0; i < ape->totalframes; i++) {
         ape->frames[i].pts = pts;
         av_add_index_entry(st, ape->frames[i].pos, ape->frames[i].pts, 0, 0, AVINDEX_KEYFRAME);
         pts += ape->blocksperframe / MAC_SUBFRAME_SIZE;
     }
 
     return 0;
 }