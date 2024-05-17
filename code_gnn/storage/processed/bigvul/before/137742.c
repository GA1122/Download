HeadlessWebContentsImpl::CreateForChildContents(
    HeadlessWebContentsImpl* parent,
    content::WebContents* child_contents) {
  auto child = base::WrapUnique(
      new HeadlessWebContentsImpl(child_contents, parent->browser_context()));

  child->begin_frame_control_enabled_ = parent->begin_frame_control_enabled_;
  child->InitializeWindow(child_contents->GetContainerBounds());

  child->mojo_services_ = parent->mojo_services_;
  if (parent->headless_tab_socket_) {
    child->headless_tab_socket_ =
        base::MakeUnique<HeadlessTabSocketImpl>(child_contents);
    child->inject_mojo_services_into_isolated_world_ =
        parent->inject_mojo_services_into_isolated_world_;
  }

  for (content::RenderFrameHost* frame_host : child_contents->GetAllFrames())
    child->RenderFrameCreated(frame_host);

  return child;
}
