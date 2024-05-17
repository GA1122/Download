void Dispatcher::WillDestroyServiceWorkerContextOnWorkerThread(
    v8::Local<v8::Context> v8_context,
    const GURL& url) {
  if (url.SchemeIs(kExtensionScheme) ||
      url.SchemeIs(kExtensionResourceScheme)) {
    g_worker_script_context_set.Get().Remove(v8_context, url);
  }
}
