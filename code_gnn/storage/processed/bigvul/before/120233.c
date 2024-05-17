void LayerTreeHost::InitializeThreaded(
    scoped_refptr<base::SingleThreadTaskRunner> impl_task_runner) {
  InitializeProxy(ThreadProxy::Create(this, impl_task_runner));
}
