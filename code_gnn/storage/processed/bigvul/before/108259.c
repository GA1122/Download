static bool isDocumentSandboxed(Frame* frame, SandboxFlags mask)
{
    return frame->document() && frame->document()->securityOrigin()->isSandboxed(mask);
}
