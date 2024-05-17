void GpuProcessHost::CallOnIO(
    GpuProcessKind kind,
    bool force_create,
    const base::Callback<void(GpuProcessHost*)>& callback) {
#if !defined(OS_WIN)
  DCHECK_NE(kind, GpuProcessHost::GPU_PROCESS_KIND_UNSANDBOXED);
#endif
  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::BindOnce(&RunCallbackOnIO, kind, force_create, callback));
}
