base::MessageLoopProxy* GetMainThreadMessageLoop() {
  CR_DEFINE_STATIC_LOCAL(scoped_refptr<base::MessageLoopProxy>, proxy,
                         (base::MessageLoopProxy::current()));
  return proxy.get();
}
