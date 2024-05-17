void BackendImpl::SetUpgradeMode() {
  user_flags_ |= kUpgradeMode;
  read_only_ = true;
}
