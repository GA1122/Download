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

  InternalDocumentStateData* internal_data =
      InternalDocumentStateData::FromDocumentLoader(
          frame_->GetDocumentLoader());
  NavigationState* navigation_state = internal_data->navigation_state();
  DCHECK(!navigation_state->WasWithinSameDocument());

  if (is_main_frame_) {
    previews_state_ =
        frame_->GetDocumentLoader()->GetRequest().GetPreviewsState();
    effective_connection_type_ =
        EffectiveConnectionTypeToWebEffectiveConnectionType(
            internal_data->effective_connection_type());
  }

  if (proxy_routing_id_ != MSG_ROUTING_NONE) {
    if (!SwapIn())
      return;
  }

  if (is_main_frame_) {
    GetRenderWidget()->DidNavigate();

    if (GetRenderWidget()->layer_tree_view())
      GetRenderWidget()->layer_tree_view()->SetURLForUkm(GetLoadingUrl());
  }

  service_manager::mojom::InterfaceProviderRequest
      remote_interface_provider_request;
  if (global_object_reuse_policy !=
      blink::WebGlobalObjectReusePolicy::kUseExisting) {
    service_manager::mojom::InterfaceProviderPtr interfaces_provider;
    remote_interface_provider_request = mojo::MakeRequest(&interfaces_provider);

    remote_interfaces_.Close();
    remote_interfaces_.Bind(std::move(interfaces_provider));

    if (auto* factory = AudioOutputIPCFactory::get()) {
      factory->MaybeDeregisterRemoteFactory(GetRoutingID());
      factory->RegisterRemoteFactory(GetRoutingID(), GetRemoteInterfaces());
    }

    audio_input_stream_factory_.reset();
  }

  if (media_permission_dispatcher_)
    media_permission_dispatcher_->OnNavigation();

  navigation_state->RunCommitNavigationCallback(blink::mojom::CommitResult::Ok);

  ui::PageTransition transition = GetTransitionType(frame_->GetDocumentLoader(),
                                                    frame_, true  );
  DidCommitNavigationInternal(item, commit_type,
                              false  , transition,
                              std::move(remote_interface_provider_request));

  if (!navigation_state->time_commit_requested().is_null()) {
    RecordReadyToCommitUntilCommitHistogram(
        base::TimeTicks::Now() - navigation_state->time_commit_requested(),
        transition);
  }

  navigation_state->set_transition_type(ui::PAGE_TRANSITION_LINK);

  UpdateEncoding(frame_, frame_->View()->PageEncoding().Utf8());

  certificate_warning_origins_.clear();
  tls_version_warning_origins_.clear();
}
