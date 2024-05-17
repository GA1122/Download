static CURLcode smtp_done(struct connectdata *conn, CURLcode status,
                          bool premature)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;
  struct SMTP *smtp = data->req.protop;
  struct pingpong *pp = &conn->proto.smtpc.pp;
  char *eob;
  ssize_t len;
  ssize_t bytes_written;

  (void)premature;

  if(!smtp || !pp->conn)
    return CURLE_OK;

   
  Curl_safefree(smtp->custom);

  if(status) {
    connclose(conn, "SMTP done with bad status");  
    result = status;          
  }
  else if(!data->set.connect_only && data->set.mail_rcpt &&
          (data->set.upload || data->set.mimepost.kind)) {
     
    if(smtp->trailing_crlf || !conn->data->state.infilesize) {
      eob = strdup(SMTP_EOB + 2);
      len = SMTP_EOB_LEN - 2;
    }
    else {
      eob = strdup(SMTP_EOB);
      len = SMTP_EOB_LEN;
    }

    if(!eob)
      return CURLE_OUT_OF_MEMORY;

     
    result = Curl_write(conn, conn->writesockfd, eob, len, &bytes_written);
    if(result) {
      free(eob);
      return result;
    }

    if(bytes_written != len) {
       
      pp->sendthis = eob;
      pp->sendsize = len;
      pp->sendleft = len - bytes_written;
    }
    else {
       
      pp->response = Curl_now();

      free(eob);
    }

    state(conn, SMTP_POSTDATA);

     
    result = smtp_block_statemach(conn);
  }

   
  smtp->transfer = FTPTRANSFER_BODY;

  return result;
}
