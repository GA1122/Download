void PrintRenderFrameHelper::SetPrintPagesParams(
    const PrintMsg_PrintPages_Params& settings) {
  print_pages_params_ = base::MakeUnique<PrintMsg_PrintPages_Params>(settings);
  Send(new PrintHostMsg_DidGetDocumentCookie(routing_id(),
                                             settings.params.document_cookie));
}
