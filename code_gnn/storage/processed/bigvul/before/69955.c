int clientsArePaused(void) {
    if (server.clients_paused &&
        server.clients_pause_end_time < server.mstime)
    {
        listNode *ln;
        listIter li;
        client *c;

        server.clients_paused = 0;

         
        listRewind(server.clients,&li);
        while ((ln = listNext(&li)) != NULL) {
            c = listNodeValue(ln);

             
            if (c->flags & (CLIENT_SLAVE|CLIENT_BLOCKED)) continue;
            c->flags |= CLIENT_UNBLOCKED;
            listAddNodeTail(server.unblocked_clients,c);
        }
    }
    return server.clients_paused;
}
