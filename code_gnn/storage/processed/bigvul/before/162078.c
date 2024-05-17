void RenderProcessHostImpl::EnableAecDumpForId(const base::FilePath& file,
                                               int id) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  base::PostTaskAndReplyWithResult(
      &GetAecDumpFileTaskRunner(), FROM_HERE,
      base::Bind(&CreateFileForProcess, file.AddExtension(IntToStringType(id))),
      base::Bind(&RenderProcessHostImpl::SendAecDumpFileToRenderer,
                 weak_factory_.GetWeakPtr(), id));
}
