static int conn_upkeep(struct connectdata *conn,
                       void *param)
{
   
  (void)param;

  if(conn->handler->connection_check) {
     
    conn->handler->connection_check(conn, CONNCHECK_KEEPALIVE);
  }

  return 0;  
}
