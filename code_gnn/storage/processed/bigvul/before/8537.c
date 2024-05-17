static void ssh2_log_outgoing_packet(Ssh ssh, struct Packet *pkt)
{
    int nblanks = 0;
    struct logblank_t blanks[4];
    char *str;
    int slen;

     
    pkt->length -= (pkt->body - pkt->data);
    pkt->savedpos = 0;

    if (ssh->logomitdata &&
        (pkt->type == SSH2_MSG_CHANNEL_DATA ||
         pkt->type == SSH2_MSG_CHANNEL_EXTENDED_DATA)) {
         
        ssh_pkt_getuint32(pkt);     
        if (pkt->type == SSH2_MSG_CHANNEL_EXTENDED_DATA)
            ssh_pkt_getuint32(pkt);     
        blanks[nblanks].offset = pkt->savedpos + 4;
        blanks[nblanks].type = PKTLOG_OMIT;
        ssh_pkt_getstring(pkt, &str, &slen);
        if (str) {
            blanks[nblanks].len = slen;
            nblanks++;
        }
    }

    if (pkt->type == SSH2_MSG_USERAUTH_REQUEST &&
        conf_get_int(ssh->conf, CONF_logomitpass)) {
         
        pkt->savedpos = 0;
        ssh_pkt_getstring(pkt, &str, &slen);
        ssh_pkt_getstring(pkt, &str, &slen);
        ssh_pkt_getstring(pkt, &str, &slen);
        if (slen == 8 && !memcmp(str, "password", 8)) {
            ssh2_pkt_getbool(pkt);
             
            blanks[nblanks].offset = pkt->savedpos;
            blanks[nblanks].type = PKTLOG_BLANK;
            ssh_pkt_getstring(pkt, &str, &slen);
            if (str) {
                blanks[nblanks].len = pkt->savedpos - blanks[nblanks].offset;
                nblanks++;
                 
                ssh_pkt_getstring(pkt, &str, &slen);
                if (str)
                    blanks[nblanks-1].len =
                        pkt->savedpos - blanks[nblanks].offset;
            }
        }
    } else if (ssh->pkt_actx == SSH2_PKTCTX_KBDINTER &&
               pkt->type == SSH2_MSG_USERAUTH_INFO_RESPONSE &&
               conf_get_int(ssh->conf, CONF_logomitpass)) {
         
        pkt->savedpos = 0;
        ssh_pkt_getuint32(pkt);
        blanks[nblanks].offset = pkt->savedpos;
        blanks[nblanks].type = PKTLOG_BLANK;
        while (1) {
            ssh_pkt_getstring(pkt, &str, &slen);
            if (!str)
                break;
        }
        blanks[nblanks].len = pkt->savedpos - blanks[nblanks].offset;
        nblanks++;
    } else if (pkt->type == SSH2_MSG_CHANNEL_REQUEST &&
               conf_get_int(ssh->conf, CONF_logomitpass)) {
         
        pkt->savedpos = 0;
        ssh_pkt_getuint32(pkt);
        ssh_pkt_getstring(pkt, &str, &slen);
        if (slen == 7 && !memcmp(str, "x11-req", 0)) {
            ssh2_pkt_getbool(pkt);
            ssh2_pkt_getbool(pkt);
            ssh_pkt_getstring(pkt, &str, &slen);
            blanks[nblanks].offset = pkt->savedpos;
            blanks[nblanks].type = PKTLOG_BLANK;
            ssh_pkt_getstring(pkt, &str, &slen);
            if (str) {
                blanks[nblanks].len = pkt->savedpos - blanks[nblanks].offset;
                nblanks++;
            }
        }
    }

    log_packet(ssh->logctx, PKT_OUTGOING, pkt->data[5],
               ssh2_pkt_type(ssh->pkt_kctx, ssh->pkt_actx, pkt->data[5]),
               pkt->body, pkt->length, nblanks, blanks,
               &ssh->v2_outgoing_sequence,
               pkt->downstream_id, pkt->additional_log_text);

     
    pkt->length += (pkt->body - pkt->data);
}
