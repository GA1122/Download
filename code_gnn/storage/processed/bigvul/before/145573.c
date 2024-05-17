void GenerateMicV2(base::span<const uint8_t, kSessionKeyLenV2> session_key,
                   base::span<const uint8_t> negotiate_msg,
                   base::span<const uint8_t> challenge_msg,
                   base::span<const uint8_t> authenticate_msg,
                   base::span<uint8_t, kMicLenV2> mic) {
  bssl::ScopedHMAC_CTX ctx;
  HMAC_Init_ex(ctx.get(), session_key.data(), kSessionKeyLenV2, EVP_md5(),
               NULL);
  DCHECK_EQ(kMicLenV2, HMAC_size(ctx.get()));
  HMAC_Update(ctx.get(), negotiate_msg.data(), negotiate_msg.size());
  HMAC_Update(ctx.get(), challenge_msg.data(), challenge_msg.size());
  HMAC_Update(ctx.get(), authenticate_msg.data(), authenticate_msg.size());
  HMAC_Final(ctx.get(), mic.data(), nullptr);
}
