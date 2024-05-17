bool TestRunner::AddFsRule(TargetPolicy::Semantics semantics,
                           const wchar_t* pattern) {
  if (!is_init_)
    return false;

  return AddRule(TargetPolicy::SUBSYS_FILES, semantics, pattern);
}
