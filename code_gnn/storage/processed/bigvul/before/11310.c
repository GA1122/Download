_gnutls_encrypt (gnutls_session_t session, const opaque * headers,
                 size_t headers_size, const opaque * data,
                 size_t data_size, opaque * ciphertext,
                 size_t ciphertext_size, content_type_t type, int random_pad,
                 record_parameters_st * params)
{
  gnutls_datum_t plain;
  gnutls_datum_t comp;
  int ret;
  int free_comp = 1;

  plain.data = (opaque *) data;
  plain.size = data_size;

  if (plain.size == 0 || is_write_comp_null (session) == 0)
    {
      comp = plain;
      free_comp = 0;
    }
  else
    {
       
      ret = _gnutls_m_plaintext2compressed (session, &comp, &plain, params);
      if (ret < 0)
        {
          gnutls_assert ();
          return ret;
        }
    }

  ret = _gnutls_compressed2ciphertext (session, &ciphertext[headers_size],
                                       ciphertext_size - headers_size,
                                       comp, type, random_pad, params);

  if (free_comp)
    _gnutls_free_datum (&comp);

  if (ret < 0)
    {
      gnutls_assert ();
      return ret;
    }


   
  memcpy (ciphertext, headers, headers_size);
  _gnutls_write_uint16 (ret, &ciphertext[3]);

  return ret + headers_size;
}