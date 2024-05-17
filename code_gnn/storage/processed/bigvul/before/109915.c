void GpuProcessHost::OnProcessCrashed(int exit_code) {
  SendOutstandingReplies();
}
