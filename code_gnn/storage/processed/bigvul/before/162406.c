MojoResult Core::FuseMessagePipes(MojoHandle handle0, MojoHandle handle1) {
  RequestContext request_context;
  scoped_refptr<Dispatcher> dispatcher0;
  scoped_refptr<Dispatcher> dispatcher1;

  bool valid_handles = true;
  {
    base::AutoLock lock(handles_->GetLock());
    MojoResult result0 =
        handles_->GetAndRemoveDispatcher(handle0, &dispatcher0);
    MojoResult result1 =
        handles_->GetAndRemoveDispatcher(handle1, &dispatcher1);
    if (result0 != MOJO_RESULT_OK || result1 != MOJO_RESULT_OK ||
        dispatcher0->GetType() != Dispatcher::Type::MESSAGE_PIPE ||
        dispatcher1->GetType() != Dispatcher::Type::MESSAGE_PIPE)
      valid_handles = false;
  }

  if (!valid_handles) {
    if (dispatcher0)
      dispatcher0->Close();
    if (dispatcher1)
      dispatcher1->Close();
    return MOJO_RESULT_INVALID_ARGUMENT;
  }

  MessagePipeDispatcher* mpd0 =
      static_cast<MessagePipeDispatcher*>(dispatcher0.get());
  MessagePipeDispatcher* mpd1 =
      static_cast<MessagePipeDispatcher*>(dispatcher1.get());

  if (!mpd0->Fuse(mpd1))
    return MOJO_RESULT_FAILED_PRECONDITION;

  return MOJO_RESULT_OK;
}
