void RenderProcessHostImpl::OnUnregisterAecDumpConsumer(int id) {
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::BindOnce(
          &RenderProcessHostImpl::UnregisterAecDumpConsumerOnUIThread,
          weak_factory_.GetWeakPtr(), id));
}
