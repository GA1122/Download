void RenderThread::OnGetRendererTcmalloc() {
  std::string result;
  char buffer[1024 * 32];
  base::ProcessId pid = base::GetCurrentProcId();
  MallocExtension::instance()->GetStats(buffer, sizeof(buffer));
  result.append(buffer);
  Send(new ViewHostMsg_RendererTcmalloc(pid, result));
}
