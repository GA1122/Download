void BrowserChildProcessHostImpl::OnProcessLaunched() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  const base::Process& process = child_process_->GetProcess();
  DCHECK(process.IsValid());

  if (child_connection_)
    child_connection_->SetProcessHandle(process.Handle());

#if defined(OS_WIN)
  DCHECK(!early_exit_watcher_.GetWatchedObject());
  early_exit_watcher_.StartWatchingOnce(process.Handle(), this);
#endif

  data_.handle = process.Handle();
  delegate_->OnProcessLaunched();

  if (is_channel_connected_) {
    ShareMetricsAllocatorToProcess();
    BrowserThread::PostTask(
        BrowserThread::UI, FROM_HERE,
        base::BindOnce(&NotifyProcessLaunchedAndConnected, data_));
  }
}
