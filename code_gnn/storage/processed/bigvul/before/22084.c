static int imap_endofresp(struct pingpong *pp, int *resp)
{
  char *line = pp->linestart_resp;
  size_t len = pp->nread_resp;
  struct imap_conn *imapc = &pp->conn->proto.imapc;
  const char *id = imapc->idstr;
  size_t id_len = strlen(id);

  if(len >= id_len + 3) {
    if(!memcmp(id, line, id_len) && (line[id_len] == ' ') ) {
       
      *resp = line[id_len+1];  
      return TRUE;
    }
    else if((imapc->state == IMAP_FETCH) &&
            !memcmp("* ", line, 2) ) {
       
      *resp = '*';
      return TRUE;
    }
  }
  return FALSE;  
}