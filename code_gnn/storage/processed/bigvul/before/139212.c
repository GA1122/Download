void RenderProcessHostImpl::Resume() {
  Send(new ChildProcessMsg_Resume());
}
