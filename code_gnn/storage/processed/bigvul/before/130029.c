VariationsSeedStore::VerifySeedSignature(
    const std::string& seed_bytes,
    const std::string& base64_seed_signature) {
  if (!SignatureVerificationEnabled())
    return VARIATIONS_SEED_SIGNATURE_ENUM_SIZE;

  if (base64_seed_signature.empty())
    return VARIATIONS_SEED_SIGNATURE_MISSING;

  std::string signature;
  if (!base::Base64Decode(base64_seed_signature, &signature))
    return VARIATIONS_SEED_SIGNATURE_DECODE_FAILED;

  crypto::SignatureVerifier verifier;
  if (!verifier.VerifyInit(
          kECDSAWithSHA256AlgorithmID, sizeof(kECDSAWithSHA256AlgorithmID),
          reinterpret_cast<const uint8*>(signature.data()), signature.size(),
          kPublicKey, arraysize(kPublicKey))) {
    return VARIATIONS_SEED_SIGNATURE_INVALID_SIGNATURE;
  }

  verifier.VerifyUpdate(reinterpret_cast<const uint8*>(seed_bytes.data()),
                        seed_bytes.size());
  if (verifier.VerifyFinal())
    return VARIATIONS_SEED_SIGNATURE_VALID;
  return VARIATIONS_SEED_SIGNATURE_INVALID_SEED;
}
