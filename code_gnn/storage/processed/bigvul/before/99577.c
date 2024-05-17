bool VerifyMAC(const std::string& key, const std::string& mac,
               const char* data, int data_length) {
  std::string key_copy = key;
  DecodeWebSafe(&key_copy);
  std::string decoded_key;
  base::Base64Decode(key_copy, &decoded_key);

  std::string mac_copy = mac;
  DecodeWebSafe(&mac_copy);
  std::string decoded_mac;
  base::Base64Decode(mac_copy, &decoded_mac);

  base::HMAC hmac(base::HMAC::SHA1);
  if (!hmac.Init(decoded_key))
    return false;
  const std::string data_str(data, data_length);
  unsigned char digest[kSafeBrowsingMacDigestSize];
  if (!hmac.Sign(data_str, digest, kSafeBrowsingMacDigestSize))
    return false;

  return !memcmp(digest, decoded_mac.data(), kSafeBrowsingMacDigestSize);
}
