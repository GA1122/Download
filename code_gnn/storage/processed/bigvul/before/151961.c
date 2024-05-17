RenderFrameHostImpl::GetFindInPage() {
  if (!find_in_page_ || !find_in_page_.is_bound() ||
      find_in_page_.encountered_error())
    GetRemoteAssociatedInterfaces()->GetInterface(&find_in_page_);
  return find_in_page_;
}
