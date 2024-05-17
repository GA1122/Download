void OptimizationHintsComponentInstallerPolicy::GetHash(
    std::vector<uint8_t>* hash) const {
  if (!hash) {
    return;
  }
  hash->assign(std::begin(kOptimizationHintsPublicKeySHA256),
               std::end(kOptimizationHintsPublicKeySHA256));
}
