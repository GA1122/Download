void LocalDOMWindow::WarnUnusedPreloads(TimerBase* base) {
  if (GetFrame() && GetFrame()->Loader().GetDocumentLoader()) {
    ResourceFetcher* fetcher =
        GetFrame()->Loader().GetDocumentLoader()->Fetcher();
    DCHECK(fetcher);
    if (fetcher->CountPreloads())
      fetcher->WarnUnusedPreloads();
  }
}
