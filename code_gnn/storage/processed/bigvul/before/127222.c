UnprivilegedProcessDelegate::~UnprivilegedProcessDelegate() {
  KillProcess(CONTROL_C_EXIT);
}
