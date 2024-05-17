bool NativeStackSamplerWin::GetModuleForHandle(
    HMODULE module_handle,
    StackSamplingProfiler::Module* module) {
  wchar_t module_name[MAX_PATH];
  DWORD result_length =
      GetModuleFileName(module_handle, module_name, arraysize(module_name));
  if (result_length == 0)
    return false;

  module->filename = base::FilePath(module_name);

  module->base_address = reinterpret_cast<uintptr_t>(module_handle);

  module->id = GetBuildIDForModule(module_handle);
  if (module->id.empty())
    return false;

  return true;
}
