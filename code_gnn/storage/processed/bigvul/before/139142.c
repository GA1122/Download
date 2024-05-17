void RenderProcessHostImpl::EnableAecDumpForId(const base::FilePath& file,
                                               int id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  BrowserThread::PostTaskAndReplyWithResult(
      BrowserThread::FILE, FROM_HERE,
      base::Bind(&CreateFileForProcess, file.AddExtension(IntToStringType(id))),
      base::Bind(&RenderProcessHostImpl::SendAecDumpFileToRenderer,
                 weak_factory_.GetWeakPtr(), id));
}
