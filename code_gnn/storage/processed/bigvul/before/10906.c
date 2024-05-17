ksba_ocsp_parse_response (ksba_ocsp_t ocsp,
                          const unsigned char *msg, size_t msglen,
                          ksba_ocsp_response_status_t *response_status)
{
  gpg_error_t err;
  struct ocsp_reqitem_s *ri;

  if (!ocsp || !msg || !msglen || !response_status)
    return gpg_error (GPG_ERR_INV_VALUE);

  if (!ocsp->requestlist)
    return gpg_error (GPG_ERR_MISSING_ACTION);

   
  ocsp->response_status = KSBA_OCSP_RSPSTATUS_NONE;
  release_ocsp_certlist (ocsp->received_certs);
  release_ocsp_extensions (ocsp->response_extensions);
  ocsp->received_certs = NULL;
  ocsp->hash_length = 0;
  ocsp->bad_nonce = 0;
  ocsp->good_nonce = 0;
  xfree (ocsp->responder_id.name);
  ocsp->responder_id.name = NULL;
  xfree (ocsp->responder_id.keyid);
  ocsp->responder_id.keyid = NULL;
  for (ri=ocsp->requestlist; ri; ri = ri->next)
    {
      ri->status = KSBA_STATUS_NONE;
      *ri->this_update = 0;
      *ri->next_update = 0;
      *ri->revocation_time = 0;
      ri->revocation_reason = 0;
      release_ocsp_extensions (ri->single_extensions);
    }

   
  err = parse_response (ocsp, msg, msglen);
  *response_status = ocsp->response_status;

   

  if (*response_status == KSBA_OCSP_RSPSTATUS_SUCCESS)
    if (ocsp->bad_nonce || (ocsp->noncelen && !ocsp->good_nonce))
      *response_status = KSBA_OCSP_RSPSTATUS_REPLAYED;

  return err;
}
