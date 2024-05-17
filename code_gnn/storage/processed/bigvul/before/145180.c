void RunCallbackOnIO(GpuProcessHost::GpuProcessKind kind,
                     bool force_create,
                     const base::Callback<void(GpuProcessHost*)>& callback) {
  GpuProcessHost* host = GpuProcessHost::Get(kind, force_create);
  callback.Run(host);
}
