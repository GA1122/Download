void PrintWebViewHelper::RenderPagesForPreview(WebFrame *frame) {
  ViewMsg_PrintPages_Params print_settings = *print_pages_params_;
  ViewHostMsg_DidPreviewDocument_Params print_params;
  CreatePreviewDocument(print_settings, frame, &print_params);
  Send(new ViewHostMsg_PagesReadyForPreview(routing_id(), print_params));
}
