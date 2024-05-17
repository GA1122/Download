void RenderFrameHostManager::SendPageMessage(IPC::Message* msg,
                                             SiteInstance* instance_to_skip) {
  DCHECK(IPC_MESSAGE_CLASS(*msg) == PageMsgStart);

  DCHECK(!frame_tree_node_->parent());

  if ((IPC_MESSAGE_CLASS(*msg) != PageMsgStart) || frame_tree_node_->parent()) {
    delete msg;
    return;
  }

  auto send_msg = [instance_to_skip](IPC::Sender* sender,
                                     int routing_id,
                                     IPC::Message* msg,
                                     SiteInstance* sender_instance) {
    if (sender_instance == instance_to_skip)
      return;

    IPC::Message* copy = new IPC::Message(*msg);
    copy->set_routing_id(routing_id);
    sender->Send(copy);
  };

  RenderFrameProxyHost* outer_delegate_proxy =
      ForInnerDelegate() ? GetProxyToOuterDelegate() : nullptr;
  for (const auto& pair : proxy_hosts_) {
    if (outer_delegate_proxy != pair.second.get()) {
      send_msg(pair.second.get(), pair.second->GetRoutingID(), msg,
               pair.second->GetSiteInstance());
    }
  }

  if (speculative_render_frame_host_) {
    send_msg(speculative_render_frame_host_.get(),
             speculative_render_frame_host_->GetRoutingID(), msg,
             speculative_render_frame_host_->GetSiteInstance());
  }

  if (render_frame_host_->GetSiteInstance() != instance_to_skip) {
    msg->set_routing_id(render_frame_host_->GetRoutingID());
    render_frame_host_->Send(msg);
  } else {
    delete msg;
  }
}
