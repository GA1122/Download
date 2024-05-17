void RenderProcessHostImpl::DumpHandles() {
#if defined(OS_WIN)
  Send(new ChildProcessMsg_DumpHandles());
  return;
#endif

  NOTIMPLEMENTED();
}
