ScopedAvoidIdentityHashForTesting::~ScopedAvoidIdentityHashForTesting() {
  converter_->avoid_identity_hash_for_testing_ = false;
}
