 static CURLcode imap_parse_url_path(struct connectdata *conn)
 {
    
    struct imap_conn *imapc = &conn->proto.imapc;
    struct SessionHandle *data = conn->data;
    const char *path = data->state.path;
  int len;
  
    if(!*path)
      path = "INBOX";
  
     
  imapc->mailbox = curl_easy_unescape(data, path, 0, &len);
  if(!imapc->mailbox)
    return CURLE_OUT_OF_MEMORY;
  return CURLE_OK;
//   return Curl_urldecode(data, path, 0, &imapc->mailbox, NULL, TRUE);
  }