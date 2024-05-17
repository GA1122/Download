void Curl_nss_close(struct connectdata *conn, int sockindex)
{
  struct ssl_connect_data *connssl = &conn->ssl[sockindex];

  if(connssl->handle) {
     
    fake_sclose(conn->sock[sockindex]);
    conn->sock[sockindex] = CURL_SOCKET_BAD;

    if((connssl->client_nickname != NULL) || (connssl->obj_clicert != NULL))
       
      SSL_InvalidateSession(connssl->handle);

    free(connssl->client_nickname);
    connssl->client_nickname = NULL;
     
    Curl_llist_destroy(connssl->obj_list, NULL);
    connssl->obj_list = NULL;
    connssl->obj_clicert = NULL;

    PR_Close(connssl->handle);
    connssl->handle = NULL;
  }
}
