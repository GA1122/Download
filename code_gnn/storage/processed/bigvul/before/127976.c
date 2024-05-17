void AwContents::CancelMIDISysexPermissionRequests(const GURL& origin) {
  permission_request_handler_->CancelRequest(
      origin, AwPermissionRequest::AwPermissionRequest::MIDISysex);
}
