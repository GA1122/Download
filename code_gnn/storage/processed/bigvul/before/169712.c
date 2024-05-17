ScopedAvoidIdentityHashForTesting::ScopedAvoidIdentityHashForTesting(
    content::V8ValueConverterImpl* converter)
    : converter_(converter) {
  CHECK(converter_);
  converter_->avoid_identity_hash_for_testing_ = true;
}
