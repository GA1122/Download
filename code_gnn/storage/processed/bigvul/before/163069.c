static void DropAllCapabilities(int proc_fd) {
  CHECK(sandbox::Credentials::DropAllCapabilities(proc_fd));
}
