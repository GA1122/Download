void NativeStackSamplerWin::CopyToSample(
    const void* const instruction_pointers[],
    const HMODULE module_handles[],
    int stack_depth,
    StackSamplingProfiler::Sample* sample,
    std::vector<StackSamplingProfiler::Module>* module) {
  sample->clear();
  sample->reserve(stack_depth);

  for (int i = 0; i < stack_depth; ++i) {
    sample->push_back(StackSamplingProfiler::Frame(
        reinterpret_cast<uintptr_t>(instruction_pointers[i]),
        GetModuleIndex(module_handles[i], module)));
  }
}
