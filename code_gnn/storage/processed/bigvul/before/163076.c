bool ZygoteMain(
    const MainFunctionParams& params,
    std::vector<std::unique_ptr<ZygoteForkDelegate>> fork_delegates) {
  g_am_zygote_or_renderer = true;

  std::vector<int> fds_to_close_post_fork;

  LinuxSandbox* linux_sandbox = LinuxSandbox::GetInstance();

  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kNoSandbox)) {
    linux_sandbox->PreinitializeSandbox();
  }

  const bool using_setuid_sandbox =
      linux_sandbox->setuid_sandbox_client()->IsSuidSandboxChild();
  const bool using_namespace_sandbox =
      sandbox::NamespaceSandbox::InNewUserNamespace();
  const bool using_layer1_sandbox =
      using_setuid_sandbox || using_namespace_sandbox;

  if (using_setuid_sandbox) {
    linux_sandbox->setuid_sandbox_client()->CloseDummyFile();
  }

  if (using_layer1_sandbox) {
    if (!base::UnixDomainSocket::SendMsg(
            kZygoteSocketPairFd, kZygoteBootMessage, sizeof(kZygoteBootMessage),
            std::vector<int>())) {
      PLOG(ERROR) << "Failed sending zygote boot message";
      _exit(1);
    }
  }

  VLOG(1) << "ZygoteMain: initializing " << fork_delegates.size()
          << " fork delegates";
  for (const auto& fork_delegate : fork_delegates) {
    fork_delegate->Init(GetSandboxFD(), using_layer1_sandbox);
  }

  const std::vector<int> sandbox_fds_to_close_post_fork =
      linux_sandbox->GetFileDescriptorsToClose();

  fds_to_close_post_fork.insert(fds_to_close_post_fork.end(),
                                sandbox_fds_to_close_post_fork.begin(),
                                sandbox_fds_to_close_post_fork.end());
  base::Closure post_fork_parent_callback =
      base::Bind(&CloseFds, fds_to_close_post_fork);

  EnterLayerOneSandbox(linux_sandbox, using_layer1_sandbox,
                       &post_fork_parent_callback);

  std::vector<pid_t> extra_children;
  std::vector<int> extra_fds;

  const int sandbox_flags = linux_sandbox->GetStatus();

  const bool setuid_sandbox_engaged = sandbox_flags & kSandboxLinuxSUID;
  CHECK_EQ(using_setuid_sandbox, setuid_sandbox_engaged);

  const bool namespace_sandbox_engaged = sandbox_flags & kSandboxLinuxUserNS;
  CHECK_EQ(using_namespace_sandbox, namespace_sandbox_engaged);

  Zygote zygote(sandbox_flags, std::move(fork_delegates), extra_children,
                extra_fds);
  return zygote.ProcessRequests();
}
