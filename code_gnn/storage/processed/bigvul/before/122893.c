bool RenderProcessHostImpl::OnMessageReceived(const IPC::Message& msg) {

  if (deleting_soon_ || fast_shutdown_started_)
    return false;

  mark_child_process_activity_time();
  if (msg.routing_id() == MSG_ROUTING_CONTROL) {
    bool msg_is_ok = true;
    IPC_BEGIN_MESSAGE_MAP_EX(RenderProcessHostImpl, msg, msg_is_ok)
      IPC_MESSAGE_HANDLER(ChildProcessHostMsg_ShutdownRequest,
                          OnShutdownRequest)
      IPC_MESSAGE_HANDLER(ChildProcessHostMsg_DumpHandlesDone,
                          OnDumpHandlesDone)
      IPC_MESSAGE_HANDLER(ViewHostMsg_SuddenTerminationChanged,
                          SuddenTerminationChanged)
      IPC_MESSAGE_HANDLER(ViewHostMsg_UserMetricsRecordAction,
                          OnUserMetricsRecordAction)
      IPC_MESSAGE_HANDLER(ViewHostMsg_SavedPageAsMHTML, OnSavedPageAsMHTML)
      IPC_MESSAGE_UNHANDLED_ERROR()
    IPC_END_MESSAGE_MAP_EX()

    if (!msg_is_ok) {
      LOG(ERROR) << "bad message " << msg.type() << " terminating renderer.";
      RecordAction(UserMetricsAction("BadMessageTerminate_BRPH"));
      ReceivedBadMessage();
    }
    return true;
  }

  RenderWidgetHost* rwh = render_widget_hosts_.Lookup(msg.routing_id());
  if (!rwh) {
    if (msg.is_sync()) {
      IPC::Message* reply = IPC::SyncMessage::GenerateReply(&msg);
      reply->set_reply_error();
      Send(reply);
    }

    bool msg_is_ok = true;
    IPC_BEGIN_MESSAGE_MAP_EX(RenderProcessHostImpl, msg, msg_is_ok)
      IPC_MESSAGE_HANDLER(ViewHostMsg_CompositorSurfaceBuffersSwapped,
                          OnCompositorSurfaceBuffersSwappedNoHost)
    IPC_END_MESSAGE_MAP_EX()
    return true;
  }
  return RenderWidgetHostImpl::From(rwh)->OnMessageReceived(msg);
}
