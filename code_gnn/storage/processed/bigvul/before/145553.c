std::string GenerateKey(base::StringPiece secret,
                        base::StringPiece salt,
                        base::StringPiece info) {
  return crypto::HkdfSha256(secret, salt, info, 32);
}
