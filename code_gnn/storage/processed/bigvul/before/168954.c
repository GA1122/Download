 void ServiceWorkerDevToolsAgentHost::DetachSession(DevToolsSession* session) {
  if (state_ == WORKER_READY && sessions().empty()) {
    BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                            base::BindOnce(&SetDevToolsAttachedOnIO,
                                           context_weak_, version_id_, false));
  }
}
