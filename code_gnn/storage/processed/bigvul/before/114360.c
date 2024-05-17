void GpuChannel::HandleMessage() {
  handle_messages_scheduled_ = false;

  if (!deferred_messages_.empty()) {
    IPC::Message* m = deferred_messages_.front();
    GpuCommandBufferStub* stub = stubs_.Lookup(m->routing_id());

    if (stub && !stub->IsScheduled()) {
      if (m->type() == GpuCommandBufferMsg_Echo::ID) {
        stub->DelayEcho(m);
        deferred_messages_.pop_front();
        if (!deferred_messages_.empty())
          OnScheduled();
      }
      return;
    }

    scoped_ptr<IPC::Message> message(m);
    deferred_messages_.pop_front();
    processed_get_state_fast_ =
        (message->type() == GpuCommandBufferMsg_GetStateFast::ID);
    if (!router_.RouteMessage(*message)) {
      if (message->is_sync()) {
        IPC::Message* reply = IPC::SyncMessage::GenerateReply(&*message);
        reply->set_reply_error();
        Send(reply);
      }
    } else {
      if (stub) {
        if (stub->HasUnprocessedCommands()) {
          deferred_messages_.push_front(new GpuCommandBufferMsg_Rescheduled(
              stub->route_id()));
        }

        ScheduleDelayedWork(stub, kHandleMoreWorkPeriodMs);
      }
    }
  }

  if (!deferred_messages_.empty()) {
    OnScheduled();
  }
}
