compute_retry_timeout(entry_connection_t *conn)
{
  int timeout = get_options()->CircuitStreamTimeout;
  if (timeout)  
    return timeout;
  if (conn->num_socks_retries < 2)  
    return 10;
  return 15;
}
