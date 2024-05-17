  bool DidRenderFrameHostRequestCommit(TestRenderFrameHost* rfh) {
    const IPC::Message* message =
        rfh->GetProcess()->sink().GetUniqueMessageMatching(
            FrameMsg_CommitNavigation::ID);
    return message && rfh->GetRoutingID() == message->routing_id();
  }
