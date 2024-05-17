static SECStatus CanFalseStartCallback(PRFileDesc *sock, void *client_data,
                                       PRBool *canFalseStart)
{
  struct connectdata *conn = client_data;
  struct Curl_easy *data = conn->data;

  SSLChannelInfo channelInfo;
  SSLCipherSuiteInfo cipherInfo;

  SECStatus rv;
  PRBool negotiatedExtension;

  *canFalseStart = PR_FALSE;

  if(SSL_GetChannelInfo(sock, &channelInfo, sizeof(channelInfo)) != SECSuccess)
    return SECFailure;

  if(SSL_GetCipherSuiteInfo(channelInfo.cipherSuite, &cipherInfo,
                            sizeof(cipherInfo)) != SECSuccess)
    return SECFailure;

   
  if(channelInfo.protocolVersion != SSL_LIBRARY_VERSION_TLS_1_2)
    goto end;

   
  if(cipherInfo.keaType != ssl_kea_ecdh)
    goto end;

   
  if(cipherInfo.symCipher != ssl_calg_aes_gcm)
    goto end;

   
  rv = SSL_HandshakeNegotiatedExtension(sock, ssl_app_layer_protocol_xtn,
                                        &negotiatedExtension);
  if(rv != SECSuccess || !negotiatedExtension) {
    rv = SSL_HandshakeNegotiatedExtension(sock, ssl_next_proto_nego_xtn,
                                          &negotiatedExtension);
  }

  if(rv != SECSuccess || !negotiatedExtension)
    goto end;

  *canFalseStart = PR_TRUE;

  infof(data, "Trying TLS False Start\n");

end:
  return SECSuccess;
}
