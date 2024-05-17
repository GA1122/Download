void RenderProcessHostImpl::UnregisterAecDumpConsumerOnUIThread(int id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  for (std::vector<int>::iterator it = aec_dump_consumers_.begin();
       it != aec_dump_consumers_.end(); ++it) {
    if (*it == id) {
      aec_dump_consumers_.erase(it);
      break;
    }
  }
}
