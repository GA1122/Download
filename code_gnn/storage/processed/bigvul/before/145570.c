void Create3DesKeysFromNtlmHash(
    base::span<const uint8_t, kNtlmHashLen> ntlm_hash,
    base::span<uint8_t, 24> keys) {
  Splay56To64(ntlm_hash.data(), keys.data());
  Splay56To64(ntlm_hash.data() + 7, keys.data() + 8);

  keys[16] = ntlm_hash[14];
  keys[17] = ntlm_hash[14] << 7 | ntlm_hash[15] >> 1;
  keys[18] = ntlm_hash[15] << 6;
  memset(keys.data() + 19, 0, 5);
}
