static int cine_read_probe(AVProbeData *p)
{
    int HeaderSize;
    if (p->buf[0] == 'C' && p->buf[1] == 'I' &&   
        (HeaderSize = AV_RL16(p->buf + 2)) >= 0x2C &&   
        AV_RL16(p->buf + 4) <= CC_UNINT &&        
        AV_RL16(p->buf + 6) <= 1 &&               
        AV_RL32(p->buf + 20) &&                   
        AV_RL32(p->buf + 24) >= HeaderSize &&     
        AV_RL32(p->buf + 28) >= HeaderSize &&     
        AV_RL32(p->buf + 32) >= HeaderSize)       
        return AVPROBE_SCORE_MAX;
    return 0;
}
