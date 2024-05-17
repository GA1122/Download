void ExtensionsGuestViewMessageFilter::CreateMimeHandlerViewGuestOnUIThread(
    int render_frame_id,
    const std::string& view_id,
    int element_instance_id,
    const gfx::Size& element_size,
    mime_handler::BeforeUnloadControlPtrInfo before_unload_control,
    int32_t plugin_frame_routing_id,
    bool is_full_page_plugin) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  auto* manager = GetOrCreateGuestViewManager();

  auto* rfh = RenderFrameHost::FromID(render_process_id_, render_frame_id);
  auto* embedder_web_contents = WebContents::FromRenderFrameHost(rfh);
  if (!embedder_web_contents)
    return;

  if (!AreRoutingIDsConsistent(rfh, plugin_frame_routing_id)) {
    bad_message::ReceivedBadMessage(rfh->GetProcess(),
                                    bad_message::MHVG_INVALID_PLUGIN_FRAME_ID);
    return;
  }

  GuestViewManager::WebContentsCreatedCallback callback = base::BindOnce(
      &ExtensionsGuestViewMessageFilter::MimeHandlerViewGuestCreatedCallback,
      this, element_instance_id, render_process_id_, render_frame_id,
      plugin_frame_routing_id, element_size, std::move(before_unload_control),
      is_full_page_plugin);

  base::DictionaryValue create_params;
  create_params.SetString(mime_handler_view::kViewId, view_id);
  create_params.SetInteger(guest_view::kElementWidth, element_size.width());
  create_params.SetInteger(guest_view::kElementHeight, element_size.height());
  manager->CreateGuest(MimeHandlerViewGuest::Type, embedder_web_contents,
                       create_params, std::move(callback));
}
