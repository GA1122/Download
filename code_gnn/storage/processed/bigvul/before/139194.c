void RenderProcessHostImpl::OnUnregisterAecDumpConsumer(int id) {
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&RenderProcessHostImpl::UnregisterAecDumpConsumerOnUIThread,
                 weak_factory_.GetWeakPtr(), id));
}
