bool ChromotingInstance::Init(uint32_t argc,
                              const char* argn[],
                              const char* argv[]) {
  CHECK(!initialized_);
  initialized_ = true;

  VLOG(1) << "Started ChromotingInstance::Init";

#if !defined(OS_NACL)
  if (!IsCallerAppOrExtension()) {
    LOG(ERROR) << "Not an app or extension";
    return false;
  }
#endif

  context_.Start();

  return true;
}
