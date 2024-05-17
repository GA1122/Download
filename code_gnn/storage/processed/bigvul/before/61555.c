static int nsv_read_packet(AVFormatContext *s, AVPacket *pkt)
{
    NSVContext *nsv = s->priv_data;
    int i, err = 0;

     
    if (!nsv->ahead[0].data && !nsv->ahead[1].data)
        err = nsv_read_chunk(s, 0);
    if (err < 0)
        return err;

     
    for (i = 0; i < 2; i++) {
        if (nsv->ahead[i].data) {
             
            memcpy(pkt, &nsv->ahead[i], sizeof(AVPacket));
            nsv->ahead[i].data = NULL;  
            return pkt->size;
        }
    }

     
    return -1;
}
