static CURLcode imap_doing(struct connectdata *conn,
                               bool *dophase_done)
{
  CURLcode result;
  result = imap_multi_statemach(conn, dophase_done);

  if(*dophase_done) {
    result = imap_dophase_done(conn, FALSE  );

    DEBUGF(infof(conn->data, "DO phase is complete\n"));
  }
  return result;
}