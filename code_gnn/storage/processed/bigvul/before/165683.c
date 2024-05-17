bool ProcessNeedsProfileDir(ProcessType process_type) {
  switch (process_type) {
    case ProcessType::BROWSER_PROCESS:
    case ProcessType::CLOUD_PRINT_SERVICE_PROCESS:
#if BUILDFLAG(ENABLE_NACL)
    case ProcessType::NACL_BROKER_PROCESS:
    case ProcessType::NACL_LOADER_PROCESS:
#endif
      return true;
    case ProcessType::OTHER_PROCESS:
      return false;
    case ProcessType::UNINITIALIZED:
      assert(false);
      return false;
  }
  assert(false);
  return false;
}
