void GenerateSessionBaseKeyV2(
    base::span<const uint8_t, kNtlmHashLen> v2_hash,
    base::span<const uint8_t, kNtlmProofLenV2> v2_proof,
    base::span<uint8_t, kSessionKeyLenV2> session_key) {
  unsigned int outlen = kSessionKeyLenV2;
  uint8_t* result =
      HMAC(EVP_md5(), v2_hash.data(), kNtlmHashLen, v2_proof.data(),
           kNtlmProofLenV2, session_key.data(), &outlen);
  DCHECK_EQ(session_key.data(), result);
  DCHECK_EQ(kSessionKeyLenV2, outlen);
}
