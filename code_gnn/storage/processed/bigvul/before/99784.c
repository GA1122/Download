bool TestRunner::AddRule(TargetPolicy::SubSystem subsystem,
                         TargetPolicy::Semantics semantics,
                         const wchar_t* pattern) {
  if (!is_init_)
    return false;

  return (SBOX_ALL_OK == policy_->AddRule(subsystem, semantics, pattern));
}
