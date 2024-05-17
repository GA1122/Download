void DomDistillerViewerSource::RequestViewerHandle::Cancel() {
  content::WebContentsObserver::Observe(NULL);

  base::MessageLoop::current()->DeleteSoon(FROM_HERE, this);
}
