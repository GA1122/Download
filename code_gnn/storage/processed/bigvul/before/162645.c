void HeadlessPrintManager::OnScriptedPrint(
    const PrintHostMsg_ScriptedPrint_Params& params,
    IPC::Message* reply_msg) {
  PageRangeStatus status =
      PageRangeTextToPages(page_ranges_text_, ignore_invalid_page_ranges_,
                           params.expected_pages_count, &print_params_->pages);
  switch (status) {
    case SYNTAX_ERROR:
      printing_rfh_->Send(reply_msg);
      ReleaseJob(PAGE_RANGE_SYNTAX_ERROR);
      return;
    case LIMIT_ERROR:
      printing_rfh_->Send(reply_msg);
      ReleaseJob(PAGE_COUNT_EXCEEDED);
      return;
    case PRINT_NO_ERROR:
      PrintHostMsg_ScriptedPrint::WriteReplyParams(reply_msg, *print_params_);
      printing_rfh_->Send(reply_msg);
      return;
    default:
      NOTREACHED();
      return;
  }
}
