void AwContents::CancelProtectedMediaIdentifierPermissionRequests(
    const GURL& origin) {
  permission_request_handler_->CancelRequest(
      origin, AwPermissionRequest::ProtectedMediaId);
}
