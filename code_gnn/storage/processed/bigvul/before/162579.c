void Resource::SetRevalidatingRequest(const ResourceRequest& request) {
  SECURITY_CHECK(redirect_chain_.IsEmpty());
  SECURITY_CHECK(!is_unused_preload_);
  DCHECK(!request.IsNull());
  CHECK(!is_revalidation_start_forbidden_);
  is_revalidating_ = true;
  resource_request_ = request;
  status_ = ResourceStatus::kNotStarted;
}
