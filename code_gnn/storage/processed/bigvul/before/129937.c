size_t NativeStackSamplerWin::GetModuleIndex(
    HMODULE module_handle,
    std::vector<StackSamplingProfiler::Module>* modules) {
  if (!module_handle)
    return StackSamplingProfiler::Frame::kUnknownModuleIndex;

  auto loc = profile_module_index_.find(module_handle);
  if (loc == profile_module_index_.end()) {
    StackSamplingProfiler::Module module;
    if (!GetModuleForHandle(module_handle, &module))
      return StackSamplingProfiler::Frame::kUnknownModuleIndex;
    modules->push_back(module);
    loc = profile_module_index_.insert(std::make_pair(
        module_handle, modules->size() - 1)).first;
  }

  return loc->second;
}
