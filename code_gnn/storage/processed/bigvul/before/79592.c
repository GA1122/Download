void imap_logout(struct ImapData **idata)
{
   
  (*idata)->status = IMAP_BYE;
  imap_cmd_start(*idata, "LOGOUT");
  if (ImapPollTimeout <= 0 || mutt_socket_poll((*idata)->conn, ImapPollTimeout) != 0)
  {
    while (imap_cmd_step(*idata) == IMAP_CMD_CONTINUE)
      ;
  }

  mutt_socket_close((*idata)->conn);
  imap_free_idata(idata);
}
