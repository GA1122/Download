BrowserChildProcessHost* BrowserChildProcessHost::Create(
    content::ProcessType process_type,
    BrowserChildProcessHostDelegate* delegate,
    const std::string& service_name) {
  return new BrowserChildProcessHostImpl(process_type, delegate, service_name);
}
