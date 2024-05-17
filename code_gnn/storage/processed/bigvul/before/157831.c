void WebContentsImpl::PausePageScheduledTasks(bool paused) {
  SendPageMessage(
      new PageMsg_PausePageScheduledTasks(MSG_ROUTING_NONE, paused));
}
