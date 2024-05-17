static int mov_probe(AVProbeData *p)
{
    int64_t offset;
    uint32_t tag;
    int score = 0;
    int moov_offset = -1;

     
    offset = 0;
    for (;;) {
         
        if ((offset + 8) > (unsigned int)p->buf_size)
            break;
        tag = AV_RL32(p->buf + offset + 4);
        switch(tag) {
         
        case MKTAG('m','o','o','v'):
            moov_offset = offset + 4;
        case MKTAG('m','d','a','t'):
        case MKTAG('p','n','o','t'):  
        case MKTAG('u','d','t','a'):  
        case MKTAG('f','t','y','p'):
            if (AV_RB32(p->buf+offset) < 8 &&
                (AV_RB32(p->buf+offset) != 1 ||
                 offset + 12 > (unsigned int)p->buf_size ||
                 AV_RB64(p->buf+offset + 8) == 0)) {
                score = FFMAX(score, AVPROBE_SCORE_EXTENSION);
            } else if (tag == MKTAG('f','t','y','p') &&
                       (   AV_RL32(p->buf + offset + 8) == MKTAG('j','p','2',' ')
                        || AV_RL32(p->buf + offset + 8) == MKTAG('j','p','x',' ')
                    )) {
                score = FFMAX(score, 5);
            } else {
                score = AVPROBE_SCORE_MAX;
            }
            offset = FFMAX(4, AV_RB32(p->buf+offset)) + offset;
            break;
         
        case MKTAG('e','d','i','w'):  
        case MKTAG('w','i','d','e'):
        case MKTAG('f','r','e','e'):
        case MKTAG('j','u','n','k'):
        case MKTAG('p','i','c','t'):
            score  = FFMAX(score, AVPROBE_SCORE_MAX - 5);
            offset = FFMAX(4, AV_RB32(p->buf+offset)) + offset;
            break;
        case MKTAG(0x82,0x82,0x7f,0x7d):
        case MKTAG('s','k','i','p'):
        case MKTAG('u','u','i','d'):
        case MKTAG('p','r','f','l'):
             
            score  = FFMAX(score, AVPROBE_SCORE_EXTENSION);
            offset = FFMAX(4, AV_RB32(p->buf+offset)) + offset;
            break;
        default:
            offset = FFMAX(4, AV_RB32(p->buf+offset)) + offset;
        }
    }
    if(score > AVPROBE_SCORE_MAX - 50 && moov_offset != -1) {
         
        offset = moov_offset;

        while(offset < (p->buf_size - 16)){  
                
            if(AV_RL32(p->buf + offset     ) == MKTAG('h','d','l','r') &&
               AV_RL32(p->buf + offset +  8) == MKTAG('m','h','l','r') &&
               AV_RL32(p->buf + offset + 12) == MKTAG('M','P','E','G')){
                av_log(NULL, AV_LOG_WARNING, "Found media data tag MPEG indicating this is a MOV-packed MPEG-PS.\n");
                 
                return 5;
            }else
                 
                offset+=2;
        }
    }

    return score;
}
