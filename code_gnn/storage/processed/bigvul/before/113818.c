void AddBaseHandleClosePolicy(sandbox::TargetPolicy* policy) {
  string16 object_path = PrependWindowsSessionPath(L"\\BaseNamedObjects");
  policy->AddKernelObjectToClose(L"Directory", object_path.data());
  object_path = PrependWindowsSessionPath(
      L"\\BaseNamedObjects\\windows_shell_global_counters");
  policy->AddKernelObjectToClose(L"Section", object_path.data());
}
