void ImageCapture::ContextDestroyed(ExecutionContext*) {
  RemoveAllEventListeners();
  service_requests_.clear();
  DCHECK(!HasEventListeners());
}
