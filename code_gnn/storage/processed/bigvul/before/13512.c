int avpriv_dv_get_packet(DVDemuxContext *c, AVPacket *pkt)
{
    int size = -1;
    int i;

    for (i = 0; i < c->ach; i++) {
       if (c->ast[i] && c->audio_pkt[i].size) {
           *pkt = c->audio_pkt[i];
           c->audio_pkt[i].size = 0;
           size = pkt->size;
           break;
       }
    }

    return size;
}
