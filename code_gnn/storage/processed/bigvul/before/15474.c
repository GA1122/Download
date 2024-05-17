create_authorization_line (const char *au, const char *user,
                           const char *passwd, const char *method,
                           const char *path, bool *finished, uerr_t *auth_err)
{
   
  switch (c_toupper (*au))
    {
    case 'B':                    
      *finished = true;
      return basic_authentication_encode (user, passwd);
#ifdef ENABLE_DIGEST
    case 'D':                    
      *finished = true;
      return digest_authentication_encode (au, user, passwd, method, path, auth_err);
#endif
#ifdef ENABLE_NTLM
    case 'N':                    
      if (!ntlm_input (&pconn.ntlm, au))
        {
          *finished = true;
          return NULL;
        }
      return ntlm_output (&pconn.ntlm, user, passwd, finished);
#endif
    default:
       
      abort ();
    }
}
