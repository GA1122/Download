void LayerTreeHost::InitializeSingleThreaded(
    LayerTreeHostSingleThreadClient* single_thread_client) {
  InitializeProxy(SingleThreadProxy::Create(this, single_thread_client));
}
