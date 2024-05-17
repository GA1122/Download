_gnutls_decrypt (gnutls_session_t session, opaque * ciphertext,
                 size_t ciphertext_size, uint8_t * data,
                 size_t max_data_size, content_type_t type,
                 record_parameters_st * params)
{
  gnutls_datum_t gtxt;
  gnutls_datum_t gcipher;
  int ret;

  if (ciphertext_size == 0)
    return 0;

  gcipher.size = ciphertext_size;
  gcipher.data = ciphertext;

  ret =
    _gnutls_ciphertext2compressed (session, data, max_data_size,
                                   gcipher, type, params);
  if (ret < 0)
    {
      return ret;
    }

  if (ret == 0 || is_read_comp_null (session) == 0)
    {
       

    }
  else
    {
      gnutls_datum_t gcomp;

       

      gcomp.data = data;
      gcomp.size = ret;
      ret = _gnutls_m_compressed2plaintext (session, &gtxt, &gcomp, params);
      if (ret < 0)
        {
          return ret;
        }

      if (gtxt.size > MAX_RECORD_RECV_SIZE)
        {
          gnutls_assert ();
          _gnutls_free_datum (&gtxt);
           
          return GNUTLS_E_DECOMPRESSION_FAILED;
        }

       
      if (max_data_size < MAX_RECORD_RECV_SIZE)
        {
          gnutls_assert ();
          _gnutls_free_datum (&gtxt);
          return GNUTLS_E_INTERNAL_ERROR;
        }

      memcpy (data, gtxt.data, gtxt.size);
      ret = gtxt.size;

      _gnutls_free_datum (&gtxt);
    }

  return ret;
}
