static CURLcode nss_fail_connect(struct ssl_connect_data *connssl,
                                 struct Curl_easy *data,
                                 CURLcode curlerr)
{
  PRErrorCode err = 0;

  if(is_nss_error(curlerr)) {
     
    err = PR_GetError();
    if(is_cc_error(err))
      curlerr = CURLE_SSL_CERTPROBLEM;

     
    infof(data, "NSS error %d (%s)\n", err, nss_error_to_name(err));

     
    nss_print_error_message(data, err);
  }

   
  Curl_llist_destroy(connssl->obj_list, NULL);
  connssl->obj_list = NULL;

  return curlerr;
}
