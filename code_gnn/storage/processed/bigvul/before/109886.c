void SecurityContext::enforceSandboxFlags(SandboxFlags mask)
{
    m_sandboxFlags |= mask;

    if (isSandboxed(SandboxOrigin) && securityOrigin() && !securityOrigin()->isUnique()) {
        setSecurityOrigin(SecurityOrigin::createUnique());
        didUpdateSecurityOrigin();
    }
}
