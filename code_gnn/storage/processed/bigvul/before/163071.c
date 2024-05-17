static void EnterNamespaceSandbox(LinuxSandbox* linux_sandbox,
                                  base::Closure* post_fork_parent_callback) {
  linux_sandbox->EngageNamespaceSandbox();

  if (getpid() == 1) {
    base::Closure drop_all_caps_callback =
        base::Bind(&DropAllCapabilities, linux_sandbox->proc_fd());
    base::Closure callback = base::Bind(
        &RunTwoClosures, &drop_all_caps_callback, post_fork_parent_callback);
    CHECK(CreateInitProcessReaper(&callback));
  }
}
