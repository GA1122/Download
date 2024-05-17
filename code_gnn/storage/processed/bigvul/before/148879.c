RenderFrameHostImpl* RenderFrameHostManager::GetFrameHostForNavigation(
    const NavigationRequest& request) {
  CHECK(IsBrowserSideNavigationEnabled());

  SiteInstance* current_site_instance = render_frame_host_->GetSiteInstance();

  SiteInstance* candidate_site_instance =
      speculative_render_frame_host_
          ? speculative_render_frame_host_->GetSiteInstance()
          : nullptr;

  bool was_server_redirect = request.navigation_handle() &&
                             request.navigation_handle()->WasServerRedirect();

  scoped_refptr<SiteInstance> dest_site_instance = GetSiteInstanceForNavigation(
      request.common_params().url, request.source_site_instance(),
      request.dest_site_instance(), candidate_site_instance,
      request.common_params().transition,
      request.restore_type() != RestoreType::NONE, request.is_view_source(),
      was_server_redirect);

  RenderFrameHostImpl* navigation_rfh = nullptr;

  bool no_renderer_swap = current_site_instance == dest_site_instance.get();

  if (frame_tree_node_->IsMainFrame()) {
    bool can_renderer_initiate_transfer =
        render_frame_host_->IsRenderFrameLive() &&
        ShouldMakeNetworkRequestForURL(request.common_params().url) &&
        IsRendererTransferNeededForNavigation(render_frame_host_.get(),
                                              request.common_params().url);

    no_renderer_swap |=
        !request.may_transfer() && !can_renderer_initiate_transfer;
  } else {
    no_renderer_swap |= !CanSubframeSwapProcess(
        request.common_params().url, request.source_site_instance(),
        request.dest_site_instance(), was_server_redirect);
  }

  bool notify_webui_of_rf_creation = false;
  if (no_renderer_swap) {
    if (speculative_render_frame_host_)
      DiscardUnusedFrame(UnsetSpeculativeRenderFrameHost());

    if (frame_tree_node_->IsMainFrame()) {
      UpdatePendingWebUIOnCurrentFrameHost(request.common_params().url,
                                           request.bindings());
    }

    navigation_rfh = render_frame_host_.get();

    DCHECK(!speculative_render_frame_host_);
  } else {

    if (!speculative_render_frame_host_ ||
        speculative_render_frame_host_->GetSiteInstance() !=
            dest_site_instance.get()) {
      CleanUpNavigation();
      bool success = CreateSpeculativeRenderFrameHost(current_site_instance,
                                                      dest_site_instance.get());
      DCHECK(success);
    }
    DCHECK(speculative_render_frame_host_);

    if (frame_tree_node_->IsMainFrame()) {
      bool changed_web_ui = speculative_render_frame_host_->UpdatePendingWebUI(
          request.common_params().url, request.bindings());
      speculative_render_frame_host_->CommitPendingWebUI();
      DCHECK_EQ(GetNavigatingWebUI(), speculative_render_frame_host_->web_ui());
      notify_webui_of_rf_creation =
          changed_web_ui && speculative_render_frame_host_->web_ui();
    }
    navigation_rfh = speculative_render_frame_host_.get();

    if (!render_frame_host_->IsRenderFrameLive()) {
      if (GetRenderFrameProxyHost(dest_site_instance.get())) {
        navigation_rfh->Send(
            new FrameMsg_SwapIn(navigation_rfh->GetRoutingID()));
      }
      CommitPending();

      if (notify_webui_of_rf_creation && render_frame_host_->web_ui()) {
        render_frame_host_->web_ui()->RenderFrameCreated(
            render_frame_host_.get());
        notify_webui_of_rf_creation = false;
      }
    }
  }
  DCHECK(navigation_rfh &&
         (navigation_rfh == render_frame_host_.get() ||
          navigation_rfh == speculative_render_frame_host_.get()));

  if (!navigation_rfh->IsRenderFrameLive()) {
    if (!ReinitializeRenderFrame(navigation_rfh))
      return nullptr;

    notify_webui_of_rf_creation = true;

    if (navigation_rfh == render_frame_host_.get()) {
      delegate_->NotifyMainFrameSwappedFromRenderManager(
          nullptr, render_frame_host_->render_view_host());
    }
  }

  if (notify_webui_of_rf_creation && GetNavigatingWebUI() &&
      frame_tree_node_->IsMainFrame()) {
    GetNavigatingWebUI()->RenderFrameCreated(navigation_rfh);
  }

  return navigation_rfh;
}
