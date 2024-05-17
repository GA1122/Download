int out_bounce_route_queue(s2s_t s2s, char *rkey, int rkeylen, int err)
{
  jqueue_t q;
  pkt_t pkt;
  int pktcount = 0;

  q = xhash_getx(s2s->outq, rkey, rkeylen);
  if(q == NULL)
     return 0;

  while((pkt = jqueue_pull(q)) != NULL) {
      
     if(pkt->nad->ecur > 1 && NAD_NURI_L(pkt->nad, NAD_ENS(pkt->nad, 1)) == strlen(uri_CLIENT) && strncmp(NAD_NURI(pkt->nad, NAD_ENS(pkt->nad, 1)), uri_CLIENT, strlen(uri_CLIENT)) == 0 && nad_find_attr(pkt->nad, 0, -1, "error", NULL) < 0) {
         sx_nad_write(s2s->router, stanza_tofrom(stanza_tofrom(stanza_error(pkt->nad, 1, err), 1), 0));
         pktcount++;
     }
     else
         nad_free(pkt->nad);

     jid_free(pkt->to);
     jid_free(pkt->from);
     free(pkt);
  }

   
  log_debug(ZONE, "deleting out packet queue for %.*s", rkeylen, rkey);
  rkey = q->key;
  jqueue_free(q);
  xhash_zap(s2s->outq, rkey);
  free(rkey);

  return pktcount;
}