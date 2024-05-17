void PrintWebViewHelper::RequestPrintPreview(PrintPreviewRequestType type) {
  const bool is_modifiable = print_preview_context_.IsModifiable();
  old_print_pages_params_.reset();
  switch (type) {
    case PRINT_PREVIEW_USER_INITIATED_ENTIRE_FRAME: {
      Send(new PrintHostMsg_RequestPrintPreview(routing_id(), is_modifiable,
                                                false));
      break;
    }
    case PRINT_PREVIEW_USER_INITIATED_CONTEXT_NODE: {
      Send(new PrintHostMsg_RequestPrintPreview(routing_id(), is_modifiable,
                                                true));
      break;
    }
    case PRINT_PREVIEW_SCRIPTED: {
      IPC::SyncMessage* msg =
          new PrintHostMsg_ScriptedPrintPreview(routing_id(), is_modifiable);
      msg->EnableMessagePumping();
      Send(msg);
      break;
    }
    default: {
      NOTREACHED();
      return;
    }
  }
}
