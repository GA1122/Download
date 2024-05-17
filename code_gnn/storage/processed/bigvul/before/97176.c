void WebFrameLoaderClient::dispatchDecidePolicyForMIMEType(
     FramePolicyFunction function,
     const String& mime_type,
     const ResourceRequest&) {
  const ResourceResponse& response =
      webframe_->frame()->loader()->activeDocumentLoader()->response();

  PolicyAction action;

  int status_code = response.httpStatusCode();
  if (status_code == 204 || status_code == 205) {
    action = PolicyIgnore;
  } else if (TreatAsAttachment(response)) {
    action = PolicyIgnore;
  } else if (!canShowMIMEType(mime_type)) {
    action = PolicyIgnore;
  } else {
    action = PolicyUse;
  }

  (webframe_->frame()->loader()->*function)(action);
}
