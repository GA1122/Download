static bool EnterSuidSandbox(sandbox::SetuidSandboxClient* setuid_sandbox,
                             base::Closure* post_fork_parent_callback) {
  DCHECK(setuid_sandbox);
  DCHECK(setuid_sandbox->IsSuidSandboxChild());


  if (!setuid_sandbox->IsSuidSandboxUpToDate()) {
    LOG(WARNING) <<
        "You are using a wrong version of the setuid binary!\n"
        "Please read "
        "https://chromium.googlesource.com/chromium/src/+/master/docs/linux_suid_sandbox_development.md."
        "\n\n";
  }

  if (!setuid_sandbox->ChrootMe())
    return false;

  if (setuid_sandbox->IsInNewPIDNamespace()) {
    CHECK_EQ(1, getpid())
        << "The SUID sandbox created a new PID namespace but Zygote "
           "is not the init process. Please, make sure the SUID "
           "binary is up to date.";
  }

  if (getpid() == 1) {
    CHECK(CreateInitProcessReaper(post_fork_parent_callback));
  }

  CHECK(SandboxDebugHandling::SetDumpableStatusAndHandlers());
  return true;
}
