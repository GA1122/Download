const ResourceRequest& Resource::LastResourceRequest() const {
  if (!redirect_chain_.size())
    return GetResourceRequest();
  return redirect_chain_.back().request_;
}
