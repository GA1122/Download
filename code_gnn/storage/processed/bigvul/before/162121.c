void RenderProcessHostImpl::OnRegisterAecDumpConsumer(int id) {
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::BindOnce(&RenderProcessHostImpl::RegisterAecDumpConsumerOnUIThread,
                     weak_factory_.GetWeakPtr(), id));
}
