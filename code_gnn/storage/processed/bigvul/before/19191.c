int out_bounce_conn_queues(conn_t out, int err)
{
  char *rkey;
  int rkeylen;
  int pktcount = 0;

   
  if (xhash_iter_first(out->routes)) {
      do {
          xhash_iter_get(out->routes, (const char **) &rkey, &rkeylen, NULL);
          pktcount += out_bounce_route_queue(out->s2s, rkey, rkeylen, err);
      } while(xhash_iter_next(out->routes));
  }

  return pktcount;
}
