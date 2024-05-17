void ChromeRenderMessageFilter::OnGetReadonlyPnaclFd(
    const std::string& filename, IPC::Message* reply_msg) {
  pnacl_file_host::GetReadonlyPnaclFd(this, filename, reply_msg);
}
