void ChromeRenderMessageFilter::OnNaClCreateTemporaryFile(
    IPC::Message* reply_msg) {
  pnacl_file_host::CreateTemporaryFile(this, reply_msg);
}
