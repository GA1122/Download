void genClientPeerId(client *client, char *peerid,
                            size_t peerid_len) {
    if (client->flags & CLIENT_UNIX_SOCKET) {
         
        snprintf(peerid,peerid_len,"%s:0",server.unixsocket);
    } else {
         
        anetFormatPeer(client->fd,peerid,peerid_len);
    }
}
