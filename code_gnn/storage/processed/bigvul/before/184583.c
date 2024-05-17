 void WebPluginDelegateProxy::SendUpdateGeometry(
     bool bitmaps_changed) {
   PluginMsg_UpdateGeometry_Param param;
   param.window_rect = plugin_rect_;
   param.clip_rect = clip_rect_;
   param.windowless_buffer0 = TransportDIB::DefaultHandleValue();
   param.windowless_buffer1 = TransportDIB::DefaultHandleValue();
   param.windowless_buffer_index = back_buffer_index();
   param.background_buffer = TransportDIB::DefaultHandleValue();
   param.transparent = transparent_;
 
 #if defined(OS_POSIX)
   if (bitmaps_changed)
 #endif
    {
      if (transport_stores_[0].dib.get())
        CopyTransportDIBHandleForMessage(transport_stores_[0].dib->handle(),
                                       &param.windowless_buffer0);
//                                        &param.windowless_buffer0,
//                                        channel_host_->peer_pid());
  
      if (transport_stores_[1].dib.get())
        CopyTransportDIBHandleForMessage(transport_stores_[1].dib->handle(),
                                       &param.windowless_buffer1);
//                                        &param.windowless_buffer1,
//                                        channel_host_->peer_pid());
  
      if (background_store_.dib.get())
        CopyTransportDIBHandleForMessage(background_store_.dib->handle(),
                                       &param.background_buffer);
//                                        &param.background_buffer,
//                                        channel_host_->peer_pid());
    }
  
    IPC::Message* msg;
 #if defined(OS_WIN)
   if (UseSynchronousGeometryUpdates()) {
     msg = new PluginMsg_UpdateGeometrySync(instance_id_, param);
   } else   
 #endif
   {
     msg = new PluginMsg_UpdateGeometry(instance_id_, param);
     msg->set_unblock(true);
   }
 
   Send(msg);
 }