void ChildThread::OnGetTcmallocStats() {
  std::string result;
  char buffer[1024 * 32];
  base::allocator::GetStats(buffer, sizeof(buffer));
  result.append(buffer);
  Send(new ChildProcessHostMsg_TcmallocStats(result));
}
