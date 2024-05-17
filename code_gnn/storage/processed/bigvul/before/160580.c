void RenderFrameImpl::DidCommitProvisionalLoad(
    const blink::WebHistoryItem& item,
    blink::WebHistoryCommitType commit_type,
    blink::WebGlobalObjectReusePolicy global_object_reuse_policy) {
  TRACE_EVENT2("navigation,rail", "RenderFrameImpl::didCommitProvisionalLoad",
               "id", routing_id_,
               "url", GetLoadingUrl().possibly_invalid_spec());
  if (!committed_first_load_ && !current_history_item_.IsNull()) {
    if (!IsMainFrame()) {
      UMA_HISTOGRAM_BOOLEAN(
          "SessionRestore.SubFrameUniqueNameChangedBeforeFirstCommit",
          name_changed_before_first_commit_);
    }
    committed_first_load_ = true;
  }

  DocumentState* document_state =
      DocumentState::FromDocumentLoader(frame_->GetDocumentLoader());
  NavigationStateImpl* navigation_state =
      static_cast<NavigationStateImpl*>(document_state->navigation_state());
  const WebURLResponse& web_url_response =
      frame_->GetDocumentLoader()->GetResponse();
  WebURLResponseExtraDataImpl* extra_data =
      GetExtraDataFromResponse(web_url_response);
  if (is_main_frame_ && !navigation_state->WasWithinSameDocument()) {
    previews_state_ = PREVIEWS_OFF;
    if (extra_data) {
      previews_state_ = extra_data->previews_state();
      effective_connection_type_ =
          EffectiveConnectionTypeToWebEffectiveConnectionType(
              extra_data->effective_connection_type());
    }
  }

  if (proxy_routing_id_ != MSG_ROUTING_NONE) {
    if (!SwapIn())
      return;
  }

  if (is_main_frame_ && !navigation_state->WasWithinSameDocument()) {
    GetRenderWidget()->DidNavigate();

    if (GetRenderWidget()->compositor())
      GetRenderWidget()->compositor()->SetURLForUkm(GetLoadingUrl());
  }

  SendUpdateState();

  service_manager::mojom::InterfaceProviderRequest
      remote_interface_provider_request;
  if (!navigation_state->WasWithinSameDocument() &&
      global_object_reuse_policy !=
          blink::WebGlobalObjectReusePolicy::kUseExisting) {
    service_manager::mojom::InterfaceProviderPtr interfaces_provider;
    remote_interface_provider_request = mojo::MakeRequest(&interfaces_provider);

    remote_interfaces_.Close();
    remote_interfaces_.Bind(std::move(interfaces_provider));

    if (auto* factory = AudioOutputIPCFactory::get()) {
      factory->MaybeDeregisterRemoteFactory(GetRoutingID());
      factory->MaybeRegisterRemoteFactory(GetRoutingID(),
                                          GetRemoteInterfaces());
    }

    audio_input_stream_factory_.reset();
  }

  if (media_permission_dispatcher_ &&
      !navigation_state->WasWithinSameDocument()) {
    media_permission_dispatcher_->OnNavigation();
  }

  UpdateStateForCommit(item, commit_type);

  GetFrameHost()->DidCommitProvisionalLoad(
      MakeDidCommitProvisionalLoadParams(commit_type),
      std::move(remote_interface_provider_request));

  navigation_state->set_transition_type(ui::PAGE_TRANSITION_LINK);

  UpdateEncoding(frame_, frame_->View()->PageEncoding().Utf8());
}
