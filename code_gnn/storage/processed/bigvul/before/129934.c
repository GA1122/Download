void FreeModuleHandles(int stack_depth, HMODULE module_handles[]) {
  for (int i = 0; i < stack_depth; ++i) {
    if (module_handles[i])
      ::FreeLibrary(module_handles[i]);
  }
}
