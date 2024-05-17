static void EnterLayerOneSandbox(LinuxSandbox* linux_sandbox,
                                 const bool using_layer1_sandbox,
                                 base::Closure* post_fork_parent_callback) {
  DCHECK(linux_sandbox);

  ZygotePreSandboxInit();

#if !defined(THREAD_SANITIZER)
  CHECK(sandbox::ThreadHelpers::IsSingleThreaded());
#endif

  sandbox::SetuidSandboxClient* setuid_sandbox =
      linux_sandbox->setuid_sandbox_client();
  if (setuid_sandbox->IsSuidSandboxChild()) {
    CHECK(EnterSuidSandbox(setuid_sandbox, post_fork_parent_callback))
        << "Failed to enter setuid sandbox";
  } else if (sandbox::NamespaceSandbox::InNewUserNamespace()) {
    EnterNamespaceSandbox(linux_sandbox, post_fork_parent_callback);
  } else {
    CHECK(!using_layer1_sandbox);
  }
}
