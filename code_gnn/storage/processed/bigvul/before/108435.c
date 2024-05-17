void ChromeRenderMessageFilter::OnLaunchNaCl(const GURL& manifest_url,
                                             int socket_count,
                                             IPC::Message* reply_msg) {
  NaClProcessHost* host = new NaClProcessHost(manifest_url, off_the_record_);
  host->Launch(this, socket_count, reply_msg, extension_info_map_);
}
