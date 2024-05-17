void RenderProcessHostImpl::OnRegisterAecDumpConsumer(int id) {
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&RenderProcessHostImpl::RegisterAecDumpConsumerOnUIThread,
                 weak_factory_.GetWeakPtr(), id));
}
