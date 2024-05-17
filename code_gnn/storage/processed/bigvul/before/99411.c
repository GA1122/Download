  SandboxIPCProcess(int lifeline_fd, int browser_socket,
                    std::string sandbox_cmd)
      : lifeline_fd_(lifeline_fd),
        browser_socket_(browser_socket),
        font_config_(new FontConfigDirect()) {
    base::InjectiveMultimap multimap;
    multimap.push_back(base::InjectionArc(0, lifeline_fd, false));
    multimap.push_back(base::InjectionArc(0, browser_socket, false));

    base::CloseSuperfluousFds(multimap);

    if (!sandbox_cmd.empty()) {
      sandbox_cmd_.push_back(sandbox_cmd);
      sandbox_cmd_.push_back(base::kFindInodeSwitch);
    }
  }
