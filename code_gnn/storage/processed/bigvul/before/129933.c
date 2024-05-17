void FindModuleHandlesForAddresses(const void* const addresses[],
                                   HMODULE module_handles[], int stack_depth) {
  for (int i = 0; i < stack_depth; ++i) {
    HMODULE module_handle = NULL;
    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
                          reinterpret_cast<LPCTSTR>(addresses[i]),
                          &module_handle)) {
      DCHECK_LE(reinterpret_cast<const void*>(module_handle), addresses[i]);
      module_handles[i] = module_handle;
    }
  }
}
