void WebFrame::setFrameOwnerSandboxFlags(WebSandboxFlags flags)
{
    FrameOwner* owner = toImplBase()->frame()->owner();
    DCHECK(owner);
    toRemoteFrameOwner(owner)->setSandboxFlags(static_cast<SandboxFlags>(flags));
}
