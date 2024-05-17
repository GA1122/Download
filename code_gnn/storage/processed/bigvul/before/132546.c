void ShellContentBrowserClient::GetAdditionalMappedFilesForChildProcess(
    const base::CommandLine& command_line,
    int child_process_id,
    FileDescriptorInfo* mappings) {
#if defined(V8_USE_EXTERNAL_STARTUP_DATA)
  if (v8_natives_fd_.get() == -1 || v8_snapshot_fd_.get() == -1) {
    int v8_natives_fd = -1;
    int v8_snapshot_fd = -1;
    if (gin::V8Initializer::OpenV8FilesForChildProcesses(&v8_natives_fd,
                                                         &v8_snapshot_fd)) {
      v8_natives_fd_.reset(v8_natives_fd);
      v8_snapshot_fd_.reset(v8_snapshot_fd);
    }
  }
  DCHECK(v8_natives_fd_.get() != -1 && v8_snapshot_fd_.get() != -1);
  mappings->Share(kV8NativesDataDescriptor, v8_natives_fd_.get());
  mappings->Share(kV8SnapshotDataDescriptor, v8_snapshot_fd_.get());
#endif   

#if defined(OS_ANDROID)
  int flags = base::File::FLAG_OPEN | base::File::FLAG_READ;
  base::FilePath pak_file;
  bool r = PathService::Get(base::DIR_ANDROID_APP_DATA, &pak_file);
  CHECK(r);
  pak_file = pak_file.Append(FILE_PATH_LITERAL("paks"));
  pak_file = pak_file.Append(FILE_PATH_LITERAL("content_shell.pak"));

  base::File f(pak_file, flags);
  if (!f.IsValid()) {
    NOTREACHED() << "Failed to open file when creating renderer process: "
                 << "content_shell.pak";
  }

  mappings->Transfer(kShellPakDescriptor, base::ScopedFD(f.TakePlatformFile()));

  if (breakpad::IsCrashReporterEnabled()) {
    f = breakpad::CrashDumpManager::GetInstance()->CreateMinidumpFile(
        child_process_id);
    if (!f.IsValid()) {
      LOG(ERROR) << "Failed to create file for minidump, crash reporting will "
                 << "be disabled for this process.";
    } else {
      mappings->Transfer(kAndroidMinidumpDescriptor,
                         base::ScopedFD(f.TakePlatformFile()));
    }
  }
#else   
  int crash_signal_fd = GetCrashSignalFD(command_line);
  if (crash_signal_fd >= 0) {
    mappings->Share(kCrashDumpSignal, crash_signal_fd);
  }
#endif   
}
