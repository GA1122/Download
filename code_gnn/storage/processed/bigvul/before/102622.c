bool CloudPolicyCacheBase::GetPublicKeyVersion(int* version) {
  if (public_key_version_.valid)
    *version = public_key_version_.version;

  return public_key_version_.valid;
}
