static void state(struct connectdata *conn, smtpstate newstate)
{
  struct smtp_conn *smtpc = &conn->proto.smtpc;
#if defined(DEBUGBUILD) && !defined(CURL_DISABLE_VERBOSE_STRINGS)
   
  static const char * const names[] = {
    "STOP",
    "SERVERGREET",
    "EHLO",
    "HELO",
    "STARTTLS",
    "UPGRADETLS",
    "AUTH",
    "COMMAND",
    "MAIL",
    "RCPT",
    "DATA",
    "POSTDATA",
    "QUIT",
     
  };

  if(smtpc->state != newstate)
    infof(conn->data, "SMTP %p state change from %s to %s\n",
          (void *)smtpc, names[smtpc->state], names[newstate]);
#endif

  smtpc->state = newstate;
}
