void BackendImpl::SetUnitTestMode() {
  user_flags_ |= kUnitTestMode;
  unit_test_ = true;
}
