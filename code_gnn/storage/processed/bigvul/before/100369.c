base::ProcessHandle BrowserRenderProcessHost::ExecuteRenderer(
    CommandLine* cmd_line,
    bool has_cmd_prefix) {
#if defined(OS_LINUX)
  if (!has_cmd_prefix) {
    base::GlobalDescriptors::Mapping mapping;
    const int ipcfd = channel_->GetClientFileDescriptor();
    mapping.push_back(std::pair<uint32_t, int>(kPrimaryIPCChannel, ipcfd));
    const int crash_signal_fd =
        Singleton<RenderCrashHandlerHostLinux>()->GetDeathSignalSocket();
    if (crash_signal_fd >= 0) {
      mapping.push_back(std::pair<uint32_t, int>(kCrashDumpSignal,
                                                 crash_signal_fd));
    }
    zygote_child_ = true;
    return Singleton<ZygoteHost>()->ForkRenderer(cmd_line->argv(), mapping);
  }
#endif   

  base::file_handle_mapping_vector fds_to_map;
  const int ipcfd = channel_->GetClientFileDescriptor();
  fds_to_map.push_back(std::make_pair(ipcfd, kPrimaryIPCChannel + 3));

#if defined(OS_LINUX)
  const int crash_signal_fd =
      Singleton<RenderCrashHandlerHostLinux>()->GetDeathSignalSocket();
  if (crash_signal_fd >= 0) {
    fds_to_map.push_back(std::make_pair(crash_signal_fd,
                                        kCrashDumpSignal + 3));
  }
  const int sandbox_fd =
      Singleton<RenderSandboxHostLinux>()->GetRendererSocket();
  fds_to_map.push_back(std::make_pair(sandbox_fd, kSandboxIPCChannel + 3));
#endif   

  zygote_child_ = false;
  base::ProcessHandle process_handle;
  if (!base::LaunchApp(cmd_line->argv(), fds_to_map, false, &process_handle))
    return 0;
  return process_handle;
}
