void Layer::RequestCopyOfOutput(
    scoped_ptr<CopyOutputRequest> request) {
  DCHECK(IsPropertyChangeAllowed());
  if (request->IsEmpty())
    return;
  copy_requests_.push_back(request.Pass());
  SetNeedsCommit();
}
