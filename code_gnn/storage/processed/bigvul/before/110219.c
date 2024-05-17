void NaClProcessHost::OnNaClGdbAttached() {
  wait_for_nacl_gdb_ = false;
  nacl_gdb_watcher_.StopWatchingFileDescriptor();
  nacl_gdb_watcher_delegate_.reset();
  OnProcessLaunched();
}
