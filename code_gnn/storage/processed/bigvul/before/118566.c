void FlagsState::ResetAllFlags(FlagsStorage* flags_storage) {
  needs_restart_ = true;

  std::set<std::string> no_experiments;
  flags_storage->SetFlags(no_experiments);
}
