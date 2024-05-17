LocalFrame* LocalFrame::Create(LocalFrameClient* client,
                               Page& page,
                               FrameOwner* owner,
                               InterfaceRegistry* interface_registry) {
  LocalFrame* frame = new LocalFrame(
      client, page, owner,
      interface_registry ? interface_registry
                         : InterfaceRegistry::GetEmptyInterfaceRegistry());
  PageScheduler* page_scheduler = page.GetPageScheduler();
  if (frame->IsMainFrame() && page_scheduler)
    page_scheduler->SetIsMainFrameLocal(true);
  probe::frameAttachedToParent(frame);
  return frame;
}
