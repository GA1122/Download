static bool smtp_endofresp(struct connectdata *conn, char *line, size_t len,
                           int *resp)
{
  struct smtp_conn *smtpc = &conn->proto.smtpc;
  bool result = FALSE;

   
  if(len < 4 || !ISDIGIT(line[0]) || !ISDIGIT(line[1]) || !ISDIGIT(line[2]))
    return FALSE;

   
  if(line[3] == ' ' || len == 5) {
    result = TRUE;
    *resp = curlx_sltosi(strtol(line, NULL, 10));

     
    if(*resp == 1)
      *resp = 0;
  }
   
  else if(line[3] == '-' &&
          (smtpc->state == SMTP_EHLO || smtpc->state == SMTP_COMMAND)) {
    result = TRUE;
    *resp = 1;   
  }

  return result;
}
