struct ImapData *imap_conn_find(const struct Account *account, int flags)
{
  struct Connection *conn = NULL;
  struct Account *creds = NULL;
  struct ImapData *idata = NULL;
  bool new = false;

  while ((conn = mutt_conn_find(conn, account)))
  {
    if (!creds)
      creds = &conn->account;
    else
      memcpy(&conn->account, creds, sizeof(struct Account));

    idata = conn->data;
    if (flags & MUTT_IMAP_CONN_NONEW)
    {
      if (!idata)
      {
         
        mutt_socket_free(conn);
        return NULL;
      }
      else if (idata->state < IMAP_AUTHENTICATED)
        continue;
    }
    if (flags & MUTT_IMAP_CONN_NOSELECT && idata && idata->state >= IMAP_SELECTED)
      continue;
    if (idata && idata->status == IMAP_FATAL)
      continue;
    break;
  }
  if (!conn)
    return NULL;  

  if (!idata)
  {
     
    idata = imap_new_idata();
    if (!idata)
    {
      mutt_socket_free(conn);
      return NULL;
    }

    conn->data = idata;
    idata->conn = conn;
    new = true;
  }

  if (idata->state == IMAP_DISCONNECTED)
    imap_open_connection(idata);
  if (idata->state == IMAP_CONNECTED)
  {
    if (imap_authenticate(idata) == IMAP_AUTH_SUCCESS)
    {
      idata->state = IMAP_AUTHENTICATED;
      FREE(&idata->capstr);
      new = true;
      if (idata->conn->ssf)
        mutt_debug(2, "Communication encrypted at %d bits\n", idata->conn->ssf);
    }
    else
      mutt_account_unsetpass(&idata->conn->account);
  }
  if (new && idata->state == IMAP_AUTHENTICATED)
  {
     
    imap_exec(idata, "CAPABILITY", IMAP_CMD_QUEUE);
     
    if (mutt_bit_isset(idata->capabilities, ENABLE))
      imap_exec(idata, "ENABLE UTF8=ACCEPT", IMAP_CMD_QUEUE);
     
    idata->delim = '/';
    imap_exec(idata, "LIST \"\" \"\"", IMAP_CMD_QUEUE);
     
    imap_exec(idata, NULL, IMAP_CMD_FAIL_OK);
  }

  return idata;
}
