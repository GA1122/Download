int SSLClientSocketOpenSSL::SelectNextProtoCallback(unsigned char** out,
                                                    unsigned char* outlen,
                                                    const unsigned char* in,
                                                    unsigned int inlen) {
#if defined(OPENSSL_NPN_NEGOTIATED)
  if (ssl_config_.next_protos.empty()) {
    *out = reinterpret_cast<uint8*>(
        const_cast<char*>(kDefaultSupportedNPNProtocol));
    *outlen = arraysize(kDefaultSupportedNPNProtocol) - 1;
    npn_status_ = kNextProtoUnsupported;
    return SSL_TLSEXT_ERR_OK;
  }

  npn_status_ = kNextProtoNoOverlap;

  for (unsigned int i = 0; i < inlen; i += in[i] + 1) {
    for (std::vector<std::string>::const_iterator
             j = ssl_config_.next_protos.begin();
         j != ssl_config_.next_protos.end(); ++j) {
      if (in[i] == j->size() &&
          memcmp(&in[i + 1], j->data(), in[i]) == 0) {
        *out = const_cast<unsigned char*>(in) + i + 1;
        *outlen = in[i];
        npn_status_ = kNextProtoNegotiated;
        break;
      }
    }
    if (npn_status_ == kNextProtoNegotiated)
      break;
  }

  if (npn_status_ == kNextProtoNoOverlap) {
    *out = reinterpret_cast<uint8*>(const_cast<char*>(
        ssl_config_.next_protos[0].data()));
    *outlen = ssl_config_.next_protos[0].size();
  }

  npn_proto_.assign(reinterpret_cast<const char*>(*out), *outlen);
  server_protos_.assign(reinterpret_cast<const char*>(in), inlen);
  DVLOG(2) << "next protocol: '" << npn_proto_ << "' status: " << npn_status_;
#endif
  return SSL_TLSEXT_ERR_OK;
}
