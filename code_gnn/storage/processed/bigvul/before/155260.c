bool GetHandlerPath(base::FilePath* exe_dir, base::FilePath* handler_path) {
#if defined(OS_ANDROID)
  if (!base::PathService::Get(base::DIR_MODULE, exe_dir)) {
    return false;
  }
  *handler_path = exe_dir->Append("libcrashpad_handler.so");
#else
  if (!base::PathService::Get(base::DIR_EXE, exe_dir)) {
    return false;
  }
  *handler_path = exe_dir->Append("crashpad_handler");
#endif
  return true;
}
