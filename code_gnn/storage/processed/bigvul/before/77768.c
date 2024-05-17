CURLcode Curl_init_userdefined(struct Curl_easy *data)
{
  struct UserDefined *set = &data->set;
  CURLcode result = CURLE_OK;

  set->out = stdout;  
  set->in_set = stdin;   
  set->err  = stderr;   

   
  set->fwrite_func = (curl_write_callback)fwrite;

   
  set->fread_func_set = (curl_read_callback)fread;
  set->is_fread_set = 0;
  set->is_fwrite_set = 0;

  set->seek_func = ZERO_NULL;
  set->seek_client = ZERO_NULL;

   
  set->convfromnetwork = ZERO_NULL;
  set->convtonetwork   = ZERO_NULL;
  set->convfromutf8    = ZERO_NULL;

  set->filesize = -1;         
  set->postfieldsize = -1;    
  set->maxredirs = -1;        

  set->httpreq = HTTPREQ_GET;  
  set->rtspreq = RTSPREQ_OPTIONS;  
  set->ftp_use_epsv = TRUE;    
  set->ftp_use_eprt = TRUE;    
  set->ftp_use_pret = FALSE;   
  set->ftp_filemethod = FTPFILE_MULTICWD;

  set->dns_cache_timeout = 60;  

   
  set->general_ssl.max_ssl_sessions = 5;

  set->proxyport = 0;
  set->proxytype = CURLPROXY_HTTP;  
  set->httpauth = CURLAUTH_BASIC;   
  set->proxyauth = CURLAUTH_BASIC;  

   
  set->socks5auth = CURLAUTH_BASIC | CURLAUTH_GSSAPI;

   
  set->hide_progress = TRUE;   

  Curl_mime_initpart(&set->mimepost, data);

   
  set->ssl.primary.verifypeer = TRUE;
  set->ssl.primary.verifyhost = TRUE;
#ifdef USE_TLS_SRP
  set->ssl.authtype = CURL_TLSAUTH_NONE;
#endif
  set->ssh_auth_types = CURLSSH_AUTH_DEFAULT;  
  set->ssl.primary.sessionid = TRUE;  
  set->proxy_ssl = set->ssl;

  set->new_file_perms = 0644;     
  set->new_directory_perms = 0755;  

   
  set->allowed_protocols = CURLPROTO_ALL;
  set->redir_protocols = CURLPROTO_ALL &   
                          ~(CURLPROTO_FILE | CURLPROTO_SCP | CURLPROTO_SMB |
                            CURLPROTO_SMBS);

#if defined(HAVE_GSSAPI) || defined(USE_WINDOWS_SSPI)
   
  set->socks5_gssapi_nec = FALSE;
#endif

   
  if(Curl_ssl_backend() != CURLSSLBACKEND_SCHANNEL) {
#if defined(CURL_CA_BUNDLE)
    result = Curl_setstropt(&set->str[STRING_SSL_CAFILE_ORIG], CURL_CA_BUNDLE);
    if(result)
      return result;

    result = Curl_setstropt(&set->str[STRING_SSL_CAFILE_PROXY],
                            CURL_CA_BUNDLE);
    if(result)
      return result;
#endif
#if defined(CURL_CA_PATH)
    result = Curl_setstropt(&set->str[STRING_SSL_CAPATH_ORIG], CURL_CA_PATH);
    if(result)
      return result;

    result = Curl_setstropt(&set->str[STRING_SSL_CAPATH_PROXY], CURL_CA_PATH);
    if(result)
      return result;
#endif
  }

  set->wildcard_enabled = FALSE;
  set->chunk_bgn      = ZERO_NULL;
  set->chunk_end      = ZERO_NULL;
  set->tcp_keepalive = FALSE;
  set->tcp_keepintvl = 60;
  set->tcp_keepidle = 60;
  set->tcp_fastopen = FALSE;
  set->tcp_nodelay = TRUE;
  set->ssl_enable_npn = TRUE;
  set->ssl_enable_alpn = TRUE;
  set->expect_100_timeout = 1000L;  
  set->sep_headers = TRUE;  
  set->buffer_size = READBUFFER_SIZE;
  set->upload_buffer_size = UPLOADBUFFER_DEFAULT;
  set->happy_eyeballs_timeout = CURL_HET_DEFAULT;
  set->fnmatch = ZERO_NULL;
  set->upkeep_interval_ms = CURL_UPKEEP_INTERVAL_DEFAULT;
  set->maxconnects = DEFAULT_CONNCACHE_SIZE;  
  set->httpversion =
#ifdef USE_NGHTTP2
    CURL_HTTP_VERSION_2TLS
#else
    CURL_HTTP_VERSION_1_1
#endif
    ;
  Curl_http2_init_userset(set);
  return result;
}
