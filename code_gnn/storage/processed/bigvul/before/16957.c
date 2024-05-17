WebContext* WebContextGetter::GetContext() const {
  DCHECK_CURRENTLY_ON(content::BrowserThread::UI);
  return context_.get();
}
