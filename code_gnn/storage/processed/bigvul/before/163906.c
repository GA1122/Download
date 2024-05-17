void ImageCapture::OnServiceConnectionError() {
  service_.reset();
  for (ScriptPromiseResolver* resolver : service_requests_)
    resolver->Reject(DOMException::Create(kNotFoundError, kNoServiceError));
  service_requests_.clear();
}
