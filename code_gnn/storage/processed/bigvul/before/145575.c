void GenerateNtlmProofV2(
    base::span<const uint8_t, kNtlmHashLen> v2_hash,
    base::span<const uint8_t, kChallengeLen> server_challenge,
    base::span<const uint8_t, kProofInputLenV2> v2_input,
    base::span<const uint8_t> target_info,
    base::span<uint8_t, kNtlmProofLenV2> v2_proof) {
  bssl::ScopedHMAC_CTX ctx;
  HMAC_Init_ex(ctx.get(), v2_hash.data(), kNtlmHashLen, EVP_md5(), NULL);
  DCHECK_EQ(kNtlmProofLenV2, HMAC_size(ctx.get()));
  HMAC_Update(ctx.get(), server_challenge.data(), kChallengeLen);
  HMAC_Update(ctx.get(), v2_input.data(), kProofInputLenV2);
  HMAC_Update(ctx.get(), target_info.data(), target_info.size());
  const uint32_t zero = 0;
  HMAC_Update(ctx.get(), reinterpret_cast<const uint8_t*>(&zero),
              sizeof(uint32_t));
  HMAC_Final(ctx.get(), v2_proof.data(), nullptr);
}
