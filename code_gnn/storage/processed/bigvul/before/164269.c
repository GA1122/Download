 int64_t ComputeResponsePadding(const std::string& response_url,
                               const crypto::SymmetricKey* padding_key,
                               bool has_metadata) {
  DCHECK(!response_url.empty());

  crypto::HMAC hmac(crypto::HMAC::SHA256);
  CHECK(hmac.Init(padding_key));

  std::string key = has_metadata ? response_url + "METADATA" : response_url;
  uint64_t digest_start;
  CHECK(hmac.Sign(key, reinterpret_cast<uint8_t*>(&digest_start),
                  sizeof(digest_start)));
  return digest_start % kPaddingRange;
}
