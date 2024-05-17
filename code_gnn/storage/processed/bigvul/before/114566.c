void RenderThreadImpl::AddObserver(content::RenderProcessObserver* observer) {
  observers_.AddObserver(observer);
}
