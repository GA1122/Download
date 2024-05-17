void RenderProcessHostImpl::OnBadMessageReceived(const IPC::Message& message) {
  auto type = message.type();
  LOG(ERROR) << "bad message " << type << " terminating renderer.";

  base::debug::Alias(&type);

  bad_message::ReceivedBadMessage(this,
                                  bad_message::RPH_DESERIALIZATION_FAILED);
}
