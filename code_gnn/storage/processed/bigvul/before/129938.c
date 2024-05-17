void NativeStackSamplerWin::ProfileRecordingStarting(
    std::vector<StackSamplingProfiler::Module>* modules) {
  current_modules_ = modules;
  profile_module_index_.clear();
}
