void ContentSecurityPolicy::EnforceSandboxFlags(SandboxFlags mask) {
  sandbox_mask_ |= mask;
}
