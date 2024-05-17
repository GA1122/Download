void ResolveNTFunctionPtr(const char* name, void* ptr) {
  HMODULE ntdll = ::GetModuleHandle(sandbox::kNtdllName);
  FARPROC* function_ptr = reinterpret_cast<FARPROC*>(ptr);
  *function_ptr = ::GetProcAddress(ntdll, name);
  if (*function_ptr)
    return;
  *function_ptr = ::GetProcAddress(ntdll, name);
  CHECK(*function_ptr);
}
