void OutOfProcessInstance::FormDidOpen(int32_t result) {
  if (result != PP_OK) {
    NOTREACHED();
  }
}