void RenderProcessHostImpl::DisableAudioDebugRecordings() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  GetAecDumpFileTaskRunner().PostTaskAndReply(
      FROM_HERE, base::BindOnce(&base::DoNothing),
      base::BindOnce(&RenderProcessHostImpl::SendDisableAecDumpToRenderer,
                     weak_factory_.GetWeakPtr()));
}
