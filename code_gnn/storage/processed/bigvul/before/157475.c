DefaultWebClientState GetDefaultWebClientStateFromShellUtilDefaultState(
    ShellUtil::DefaultState default_state) {
  switch (default_state) {
    case ShellUtil::UNKNOWN_DEFAULT:
      return DefaultWebClientState::UNKNOWN_DEFAULT;
    case ShellUtil::NOT_DEFAULT:
      return DefaultWebClientState::NOT_DEFAULT;
    case ShellUtil::IS_DEFAULT:
      return DefaultWebClientState::IS_DEFAULT;
    case ShellUtil::OTHER_MODE_IS_DEFAULT:
      return DefaultWebClientState::OTHER_MODE_IS_DEFAULT;
  }
  NOTREACHED();
  return DefaultWebClientState::UNKNOWN_DEFAULT;
}
