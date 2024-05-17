void PwnMessageHelper::RegisterBlobURL(RenderProcessHost* process,
                                       GURL url,
                                       std::string uuid) {
  IPC::IpcSecurityTestUtil::PwnMessageReceived(
      process->GetChannel(), BlobHostMsg_RegisterPublicURL(url, uuid));
}
