static bool extract_if_dead(struct connectdata *conn,
                            struct Curl_easy *data)
{
  size_t pipeLen = conn->send_pipe.size + conn->recv_pipe.size;
  if(!pipeLen && !CONN_INUSE(conn)) {
     
    bool dead;

    conn->data = data;
    if(conn->handler->connection_check) {
       
      unsigned int state;

      state = conn->handler->connection_check(conn, CONNCHECK_ISDEAD);
      dead = (state & CONNRESULT_DEAD);
    }
    else {
       
      dead = SocketIsDead(conn->sock[FIRSTSOCKET]);
    }

    if(dead) {
      infof(data, "Connection %ld seems to be dead!\n", conn->connection_id);
      Curl_conncache_remove_conn(conn, FALSE);
      conn->data = NULL;  
      return TRUE;
    }
  }
  return FALSE;
}
