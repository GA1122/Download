rend_service_free_intro(rend_intro_cell_t *request)
{
  if (!request) {
    return;
  }

   
  tor_free(request->ciphertext);
  request->ciphertext_len = 0;

   
  if (request->plaintext) {
     
    memwipe(request->plaintext, 0, request->plaintext_len);
    tor_free(request->plaintext);
    request->plaintext_len = 0;
  }

   
  if (request->parsed) {
    switch (request->version) {
      case 0:
      case 1:
         
        break;
      case 2:
        extend_info_free(request->u.v2.extend_info);
        request->u.v2.extend_info = NULL;
        break;
      case 3:
        if (request->u.v3.auth_data) {
          memwipe(request->u.v3.auth_data, 0, request->u.v3.auth_len);
          tor_free(request->u.v3.auth_data);
        }

        extend_info_free(request->u.v3.extend_info);
        request->u.v3.extend_info = NULL;
        break;
      default:
        log_info(LD_BUG,
                 "rend_service_free_intro() saw unknown protocol "
                 "version %d.",
                 request->version);
    }
  }

   
  memwipe(request, 0, sizeof(*request));

  tor_free(request);
}
