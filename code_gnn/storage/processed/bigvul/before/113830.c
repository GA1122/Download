BOOL WINAPI DuplicateHandlePatch(HANDLE source_process_handle,
                                 HANDLE source_handle,
                                 HANDLE target_process_handle,
                                 LPHANDLE target_handle,
                                 DWORD desired_access,
                                 BOOL inherit_handle,
                                 DWORD options) {
  if (!g_iat_orig_duplicate_handle(source_process_handle, source_handle,
                                   target_process_handle, target_handle,
                                   desired_access, inherit_handle, options))
    return FALSE;

  if (source_process_handle == target_process_handle ||
      target_process_handle == ::GetCurrentProcess())
    return TRUE;

  BOOL is_in_job = FALSE;
  if (!::IsProcessInJob(target_process_handle, NULL, &is_in_job)) {
    if (ERROR_ACCESS_DENIED == ::GetLastError()) {
      base::win::ScopedHandle process;
      CHECK(g_iat_orig_duplicate_handle(::GetCurrentProcess(),
                                        target_process_handle,
                                        ::GetCurrentProcess(),
                                        process.Receive(),
                                        PROCESS_QUERY_INFORMATION,
                                        FALSE, 0));
      CHECK(::IsProcessInJob(process, NULL, &is_in_job));
    }
  }

  if (is_in_job) {
    CHECK(!inherit_handle) << kDuplicateHandleWarning;

    base::win::ScopedHandle handle;
    CHECK(g_iat_orig_duplicate_handle(target_process_handle, *target_handle,
                                      ::GetCurrentProcess(), handle.Receive(),
                                      0, FALSE, DUPLICATE_SAME_ACCESS));

    CheckDuplicateHandle(handle);
  }

  return TRUE;
}
