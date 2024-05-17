FilePath::StringType NaClProcessHost::GetIrtLibraryFilename() {
  bool on_x86_64 = running_on_wow64_;
#if defined(__x86_64__)
  on_x86_64 = true;
#endif
  if (on_x86_64) {
    return FILE_PATH_LITERAL("nacl_irt_x86_64.nexe");
  } else {
    return FILE_PATH_LITERAL("nacl_irt_x86_32.nexe");
  }
}
