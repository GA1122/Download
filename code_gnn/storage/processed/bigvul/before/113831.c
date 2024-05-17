bool InitBrokerServices(sandbox::BrokerServices* broker_services) {
  DCHECK(broker_services);
  DCHECK(!g_broker_services);
  sandbox::ResultCode result = broker_services->Init();
  g_broker_services = broker_services;

  BOOL is_in_job = FALSE;
#ifdef NACL_WIN64
  CHECK(::IsProcessInJob(::GetCurrentProcess(), NULL, &is_in_job));
#endif
#ifndef OFFICIAL_BUILD
  if (!is_in_job && !g_iat_patch_duplicate_handle.is_patched()) {
    HMODULE module = NULL;
    wchar_t module_name[MAX_PATH];
    CHECK(::GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                              reinterpret_cast<LPCWSTR>(InitBrokerServices),
                              &module));
    DWORD result = ::GetModuleFileNameW(module, module_name, MAX_PATH);
    if (result && (result != MAX_PATH)) {
      ResolveNTFunctionPtr("NtQueryObject", &g_QueryObject);
      g_iat_orig_duplicate_handle = ::DuplicateHandle;
      g_iat_patch_duplicate_handle.Patch(
          module_name, "kernel32.dll", "DuplicateHandle",
          DuplicateHandlePatch);
    }
  }
#endif

  return SBOX_ALL_OK == result;
}
