void RenderThreadImpl::RemoveObserver(
    content::RenderProcessObserver* observer) {
  observers_.RemoveObserver(observer);
}
