ksba_ocsp_release (ksba_ocsp_t ocsp)
{
  struct ocsp_reqitem_s *ri;

  if (!ocsp)
    return;
  xfree (ocsp->digest_oid);
  xfree (ocsp->request_buffer);
  for (; (ri=ocsp->requestlist); ri = ocsp->requestlist )
    {
      ocsp->requestlist = ri->next;
      ksba_cert_release (ri->cert);
      ksba_cert_release (ri->issuer_cert);
      release_ocsp_extensions (ri->single_extensions);
      xfree (ri->serialno);
    }
  xfree (ocsp->sigval);
  xfree (ocsp->responder_id.name);
  xfree (ocsp->responder_id.keyid);
  release_ocsp_certlist (ocsp->received_certs);
  release_ocsp_extensions (ocsp->response_extensions);
  xfree (ocsp);
}
