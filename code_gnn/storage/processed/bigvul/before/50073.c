bool Curl_nss_false_start(void) {
#if NSSVERNUM >= 0x030f04  
  return TRUE;
#else
  return FALSE;
#endif
}
