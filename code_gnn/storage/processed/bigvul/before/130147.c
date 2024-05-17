bool Initialize(bool force) {
  if (!InitializeInterceptImports())
    return false;

  if (IsNonBrowserProcess())
    return false;

  if (!force && !LeaveSetupBeacon())
    return false;

  const bool kRelaxed = false;

  sandbox::ServiceResolverThunk* thunk = GetThunk(kRelaxed);

  if (!thunk)
    return false;

  BYTE* thunk_storage = reinterpret_cast<BYTE*>(&g_thunk_storage);

  DWORD old_protect = 0;
  if (!VirtualProtect(&g_thunk_storage,
                      sizeof(g_thunk_storage),
                      PAGE_EXECUTE_READWRITE,
                      &old_protect)) {
    return false;
  }

  thunk->AllowLocalPatches();

  BOOL page_executable = false;

#if defined(_WIN64)
  NTSTATUS ret = thunk->Setup(::GetModuleHandle(sandbox::kNtdllName),
                              reinterpret_cast<void*>(&__ImageBase),
                              "NtMapViewOfSection",
                              NULL,
                              &blacklist::BlNtMapViewOfSection64,
                              thunk_storage,
                              sizeof(sandbox::ThunkData),
                              NULL);

  g_nt_map_view_of_section_func = reinterpret_cast<NtMapViewOfSectionFunction>(
      thunk_storage);

 page_executable = VirtualProtect(&g_nt_map_view_of_section_func,
                                  sizeof(g_nt_map_view_of_section_func),
                                  PAGE_EXECUTE_READ,
                                  &old_protect);
#else
  NTSTATUS ret = thunk->Setup(::GetModuleHandle(sandbox::kNtdllName),
                              reinterpret_cast<void*>(&__ImageBase),
                              "NtMapViewOfSection",
                              NULL,
                              &blacklist::BlNtMapViewOfSection,
                              thunk_storage,
                              sizeof(sandbox::ThunkData),
                              NULL);
#endif
  delete thunk;

  g_blacklist_initialized = NT_SUCCESS(ret);

  page_executable = page_executable && VirtualProtect(&g_thunk_storage,
                                                      sizeof(g_thunk_storage),
                                                      PAGE_EXECUTE_READ,
                                                      &old_protect);

  AddDllsFromRegistryToBlacklist();

  return NT_SUCCESS(ret) && page_executable;
}
