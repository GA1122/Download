void AwContents::RequestMIDISysexPermission(
    const GURL& origin,
    const base::Callback<void(bool)>& callback) {
  permission_request_handler_->SendRequest(
      scoped_ptr<AwPermissionRequestDelegate>(new SimplePermissionRequest(
          origin, AwPermissionRequest::MIDISysex, callback)));
}
