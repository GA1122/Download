int btif_hl_update_maxfd(int max_org_s) {
 int maxfd = max_org_s;

    BTIF_TRACE_DEBUG("btif_hl_update_maxfd max_org_s= %d", max_org_s);
 for (const list_node_t *node = list_begin(soc_queue);
            node != list_end(soc_queue); node = list_next(node)) {
 btif_hl_soc_cb_t *p_scb = list_node(node);
 if (maxfd < p_scb->max_s) {
            maxfd = p_scb->max_s;
            BTIF_TRACE_DEBUG("btif_hl_update_maxfd maxfd=%d", maxfd);
 }
 }

    BTIF_TRACE_DEBUG("btif_hl_update_maxfd final *p_max_s=%d", maxfd);
 return maxfd;
}
