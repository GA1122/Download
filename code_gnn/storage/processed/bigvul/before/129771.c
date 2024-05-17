void ChildThread::OnDumpHandles() {
#if defined(OS_WIN)
  scoped_refptr<HandleEnumerator> handle_enum(
      new HandleEnumerator(
          CommandLine::ForCurrentProcess()->HasSwitch(
              switches::kAuditAllHandles)));
  handle_enum->EnumerateHandles();
  Send(new ChildProcessHostMsg_DumpHandlesDone);
#else
  NOTIMPLEMENTED();
#endif
}
