RemoteFrame* RemoteFrame::Create(RemoteFrameClient* client,
                                 Page& page,
                                 FrameOwner* owner) {
  return new RemoteFrame(client, page, owner);
}
