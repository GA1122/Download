void ResourceMessageFilter::OnReceiveContextMenuMsg(const IPC::Message& msg) {
  void* iter = NULL;
  ContextMenuParams params;
  if (!IPC::ParamTraits<ContextMenuParams>::Read(&msg, &iter, &params))
    return;

  const ViewHostMsg_ContextMenu context_menu_message(msg.routing_id(), params);
  ChromeThread::PostTask(
      ChromeThread::UI, FROM_HERE,
      new ContextMenuMessageDispatcher(id(), context_menu_message));
}
