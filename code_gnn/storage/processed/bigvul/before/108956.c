void RenderViewImpl::OnPpapiBrokerPermissionResult(
    int request_id,
    bool result) {
  pepper_delegate_.OnPpapiBrokerPermissionResult(request_id, result);
}
