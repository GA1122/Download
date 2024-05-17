bool MimeHandlerViewContainer::OnMessage(const IPC::Message& message) {
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(MimeHandlerViewContainer, message)
  IPC_MESSAGE_HANDLER(ExtensionsGuestViewMsg_CreateMimeHandlerViewGuestACK,
                      OnCreateMimeHandlerViewGuestACK)
  IPC_MESSAGE_HANDLER(
      ExtensionsGuestViewMsg_MimeHandlerViewGuestOnLoadCompleted,
      OnMimeHandlerViewGuestOnLoadCompleted)
  IPC_MESSAGE_HANDLER(GuestViewMsg_GuestAttached, OnGuestAttached)
  IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  return handled;
}
