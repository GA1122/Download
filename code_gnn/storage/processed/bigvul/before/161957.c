void PrintRenderFrameHelper::RequestPrintPreview(PrintPreviewRequestType type) {
  auto weak_this = weak_ptr_factory_.GetWeakPtr();
  print_preview_context_.source_frame()->DispatchBeforePrintEvent();
  if (!weak_this)
    return;
  const bool is_modifiable = print_preview_context_.IsModifiable();
  const bool has_selection = print_preview_context_.HasSelection();
  PrintHostMsg_RequestPrintPreview_Params params;
  params.is_modifiable = is_modifiable;
  params.has_selection = has_selection;
  switch (type) {
    case PRINT_PREVIEW_SCRIPTED: {
      is_scripted_preview_delayed_ = true;
      if (is_loading_ && GetPlugin(print_preview_context_.source_frame())) {
        on_stop_loading_closure_ =
            base::Bind(&PrintRenderFrameHelper::ShowScriptedPrintPreview,
                       weak_ptr_factory_.GetWeakPtr());
      } else {
        base::ThreadTaskRunnerHandle::Get()->PostTask(
            FROM_HERE,
            base::Bind(&PrintRenderFrameHelper::ShowScriptedPrintPreview,
                       weak_ptr_factory_.GetWeakPtr()));
      }
      auto msg = base::MakeUnique<PrintHostMsg_SetupScriptedPrintPreview>(
          routing_id());
      msg->EnableMessagePumping();
      auto self = weak_ptr_factory_.GetWeakPtr();
      Send(msg.release());
      if (self)
        is_scripted_preview_delayed_ = false;
      return;
    }
    case PRINT_PREVIEW_USER_INITIATED_ENTIRE_FRAME: {
      if (is_loading_ && GetPlugin(print_preview_context_.source_frame())) {
        on_stop_loading_closure_ =
            base::Bind(&PrintRenderFrameHelper::RequestPrintPreview,
                       weak_ptr_factory_.GetWeakPtr(), type);
        return;
      }

      break;
    }
    case PRINT_PREVIEW_USER_INITIATED_SELECTION: {
      DCHECK(has_selection);
      DCHECK(!GetPlugin(print_preview_context_.source_frame()));
      params.selection_only = has_selection;
      break;
    }
    case PRINT_PREVIEW_USER_INITIATED_CONTEXT_NODE: {
      if (is_loading_ && GetPlugin(print_preview_context_.source_frame())) {
        on_stop_loading_closure_ =
            base::Bind(&PrintRenderFrameHelper::RequestPrintPreview,
                       weak_ptr_factory_.GetWeakPtr(), type);
        return;
      }

      params.webnode_only = true;
      break;
    }
    default: {
      NOTREACHED();
      return;
    }
  }
  Send(new PrintHostMsg_RequestPrintPreview(routing_id(), params));
}
