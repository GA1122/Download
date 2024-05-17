void ExtensionsGuestViewMessageFilter::CreateMimeHandlerViewGuest(
    int32_t render_frame_id,
    const std::string& view_id,
    int32_t element_instance_id,
    const gfx::Size& element_size,
    mime_handler::BeforeUnloadControlPtr before_unload_control,
    int32_t plugin_frame_routing_id) {
  base::PostTaskWithTraits(
      FROM_HERE, {content::BrowserThread::UI},
      base::BindOnce(&ExtensionsGuestViewMessageFilter::
                         CreateMimeHandlerViewGuestOnUIThread,
                     this, render_frame_id, view_id, element_instance_id,
                     element_size, before_unload_control.PassInterface(),
                     plugin_frame_routing_id, false));
}
