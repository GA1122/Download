base::Process ProcessFromHandle(base::ProcessHandle handle) {
#if defined(OS_WIN)
  if (handle == GetCurrentProcess())
    return base::Process::Current();

  base::ProcessHandle out_handle;
  if (!::DuplicateHandle(GetCurrentProcess(), handle, GetCurrentProcess(),
                         &out_handle, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
    return base::Process();
  }
  handle = out_handle;
#endif   
  return base::Process(handle);
}
