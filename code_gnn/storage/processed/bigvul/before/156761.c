RemoteFrame* RemoteFrame::Create(RemoteFrameClient* client,
                                 Page& page,
                                 FrameOwner* owner) {
  RemoteFrame* frame = new RemoteFrame(client, page, owner);
  PageScheduler* page_scheduler = page.GetPageScheduler();
  if (frame->IsMainFrame() && page_scheduler)
    page_scheduler->SetIsMainFrameLocal(false);
  return frame;
}
