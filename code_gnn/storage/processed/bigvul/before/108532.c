QuotaManagerProxy::QuotaManagerProxy(
    QuotaManager* manager, base::SingleThreadTaskRunner* io_thread)
    : manager_(manager), io_thread_(io_thread) {
}
