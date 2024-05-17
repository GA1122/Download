static void on_timer(nw_timer *timer, void *privdata)
{
   ws_svr *svr = privdata;
   double now = current_timestamp();

   nw_ses *curr = svr->raw_svr->clt_list_head;
   nw_ses *next;
   while (curr) {
       next = curr->next;
       struct clt_info *info = curr->privdata;
       if (now - info->last_activity > svr->keep_alive) {
           log_error("peer: %s: last_activity: %f, idle too long", nw_sock_human_addr(&curr->peer_addr), info->last_activity);
           nw_svr_close_clt(svr->raw_svr, curr);
       }
       curr = next;
   }
}
