uint32 PluginResponseUtils::CPLoadFlagsToNetFlags(uint32 flags) {
  uint32 net_flags = 0;
#define HANDLE_FLAG(name) \
  if (flags & CPREQUEST##name) \
  net_flags |= net::name

  HANDLE_FLAG(LOAD_VALIDATE_CACHE);
  HANDLE_FLAG(LOAD_BYPASS_CACHE);
  HANDLE_FLAG(LOAD_PREFERRING_CACHE);
  HANDLE_FLAG(LOAD_ONLY_FROM_CACHE);
  HANDLE_FLAG(LOAD_DISABLE_CACHE);
  HANDLE_FLAG(LOAD_DISABLE_INTERCEPT);

  net_flags |= net::LOAD_ENABLE_UPLOAD_PROGRESS;

  return net_flags;
}
