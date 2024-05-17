void ResourceMessageFilter::OnGetCPBrowsingContext(uint32* context) {
  *context = CPBrowsingContextManager::Instance()->Allocate(
      request_context_->GetURLRequestContext());
}
