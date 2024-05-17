int SSLClientSocketOpenSSL::DoVerifyCertComplete(int result) {
  verifier_.reset();

  if (result == OK) {
    SSLContext::GetInstance()->session_cache()->MarkSSLSessionAsGood(ssl_);
  } else {
    DVLOG(1) << "DoVerifyCertComplete error " << ErrorToString(result)
             << " (" << result << ")";
  }

  completed_handshake_ = true;
  DCHECK_EQ(STATE_NONE, next_handshake_state_);
  return result;
}
