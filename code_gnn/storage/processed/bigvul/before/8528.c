static void ssh1_log_outgoing_packet(Ssh ssh, struct Packet *pkt)
{
    int nblanks = 0;
    struct logblank_t blanks[4];
    char *str;
    int slen;

     
    pkt->length -= (pkt->body - pkt->data);
    pkt->savedpos = 0;

    if (ssh->logomitdata &&
        (pkt->type == SSH1_CMSG_STDIN_DATA ||
         pkt->type == SSH1_MSG_CHANNEL_DATA)) {
         
        if (pkt->type == SSH1_MSG_CHANNEL_DATA)
            ssh_pkt_getuint32(pkt);     
        blanks[nblanks].offset = pkt->savedpos + 4;
        blanks[nblanks].type = PKTLOG_OMIT;
        ssh_pkt_getstring(pkt, &str, &slen);
        if (str) {
            blanks[nblanks].len = slen;
            nblanks++;
        }
    }

    if ((pkt->type == SSH1_CMSG_AUTH_PASSWORD ||
         pkt->type == SSH1_CMSG_AUTH_TIS_RESPONSE ||
         pkt->type == SSH1_CMSG_AUTH_CCARD_RESPONSE) &&
        conf_get_int(ssh->conf, CONF_logomitpass)) {
         
        blanks[nblanks].offset = 0;
        blanks[nblanks].len = pkt->length;
        blanks[nblanks].type = PKTLOG_BLANK;
        nblanks++;
    } else if (pkt->type == SSH1_CMSG_X11_REQUEST_FORWARDING &&
               conf_get_int(ssh->conf, CONF_logomitpass)) {
         
        pkt->savedpos = 0;
        ssh_pkt_getstring(pkt, &str, &slen);
        blanks[nblanks].offset = pkt->savedpos;
        blanks[nblanks].type = PKTLOG_BLANK;
        ssh_pkt_getstring(pkt, &str, &slen);
        if (str) {
            blanks[nblanks].len = pkt->savedpos - blanks[nblanks].offset;
            nblanks++;
        }
    }

    log_packet(ssh->logctx, PKT_OUTGOING, pkt->data[12],
               ssh1_pkt_type(pkt->data[12]),
               pkt->body, pkt->length,
               nblanks, blanks, NULL, 0, NULL);

     
    pkt->length += (pkt->body - pkt->data);
}
