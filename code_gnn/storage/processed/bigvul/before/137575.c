void PrintWebViewHelper::SetPrintPagesParams(
    const PrintMsg_PrintPages_Params& settings) {
  print_pages_params_.reset(new PrintMsg_PrintPages_Params(settings));
  Send(new PrintHostMsg_DidGetDocumentCookie(routing_id(),
                                             settings.params.document_cookie));
}
