ImageCapture::~ImageCapture() {
  DCHECK(!HasEventListeners());
  DCHECK(service_requests_.IsEmpty());
}
