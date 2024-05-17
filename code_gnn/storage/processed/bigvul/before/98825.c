void WebPluginDelegateProxy::WindowFrameChanged(gfx::Rect window_frame,
                                                gfx::Rect view_frame) {
  IPC::Message* msg = new PluginMsg_WindowFrameChanged(instance_id_,
                                                       window_frame,
                                                       view_frame);
  msg->set_unblock(true);
  Send(msg);
}
