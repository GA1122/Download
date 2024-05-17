void SupervisedUserService::OnBlacklistLoaded() {
  DCHECK(blacklist_state_ == BlacklistLoadState::LOAD_STARTED);
  blacklist_state_ = BlacklistLoadState::LOADED;
  UpdateBlacklist();
}
