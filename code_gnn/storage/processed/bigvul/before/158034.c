NavigationPolicy LocalFrameClientImpl::DecidePolicyForNavigation(
    const ResourceRequest& request,
    Document* origin_document,
    DocumentLoader* document_loader,
    WebNavigationType type,
    NavigationPolicy policy,
    bool replaces_current_history_item,
    bool is_client_redirect,
    WebTriggeringEventInfo triggering_event_info,
    HTMLFormElement* form,
    ContentSecurityPolicyDisposition
        should_check_main_world_content_security_policy,
    mojom::blink::BlobURLTokenPtr blob_url_token,
    base::TimeTicks input_start_time) {
  if (!web_frame_->Client())
    return kNavigationPolicyIgnore;

  WebDocumentLoaderImpl* web_document_loader =
      WebDocumentLoaderImpl::FromDocumentLoader(document_loader);

  WrappedResourceRequest wrapped_resource_request(request);
  WebLocalFrameClient::NavigationPolicyInfo navigation_info(
      wrapped_resource_request);
  navigation_info.navigation_type = type;
  navigation_info.default_policy = static_cast<WebNavigationPolicy>(policy);
  CHECK(!web_document_loader || !web_document_loader->GetExtraData());
  navigation_info.replaces_current_history_item = replaces_current_history_item;
  navigation_info.is_client_redirect = is_client_redirect;
  navigation_info.triggering_event_info = triggering_event_info;
  navigation_info.should_check_main_world_content_security_policy =
      should_check_main_world_content_security_policy ==
              kCheckContentSecurityPolicy
          ? kWebContentSecurityPolicyDispositionCheck
          : kWebContentSecurityPolicyDispositionDoNotCheck;
  navigation_info.blob_url_token = blob_url_token.PassInterface().PassHandle();
  navigation_info.input_start = input_start_time;

  LocalFrame* local_parent_frame = GetLocalParentFrame(web_frame_);

  navigation_info.is_history_navigation_in_new_child_frame =
      IsBackForwardNavigationInProgress(local_parent_frame);

  navigation_info.archive_status =
      IsLoadedAsMHTMLArchive(local_parent_frame)
          ? WebLocalFrameClient::NavigationPolicyInfo::ArchiveStatus::Present
          : WebLocalFrameClient::NavigationPolicyInfo::ArchiveStatus::Absent;

  if (form)
    navigation_info.form = WebFormElement(form);

  std::unique_ptr<SourceLocation> source_location =
      origin_document
          ? SourceLocation::Capture(origin_document)
          : SourceLocation::Capture(web_frame_->GetFrame()->GetDocument());
  if (source_location && !source_location->IsUnknown()) {
    navigation_info.source_location.url = source_location->Url();
    navigation_info.source_location.line_number = source_location->LineNumber();
    navigation_info.source_location.column_number =
        source_location->ColumnNumber();
  }

  if (WebDevToolsAgentImpl* devtools = DevToolsAgent()) {
    navigation_info.devtools_initiator_info =
        devtools->NavigationInitiatorInfo(web_frame_->GetFrame());
  }

  WebNavigationPolicy web_policy =
      web_frame_->Client()->DecidePolicyForNavigation(navigation_info);
  return static_cast<NavigationPolicy>(web_policy);
}
