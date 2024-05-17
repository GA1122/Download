void ContentSecurityPolicy::enforceSandboxFlags(SandboxFlags mask) {
  m_sandboxMask |= mask;
}
