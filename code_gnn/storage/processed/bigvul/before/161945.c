void PrintRenderFrameHelper::Print(blink::WebLocalFrame* frame,
                                   const blink::WebNode& node,
                                   bool is_scripted) {
  if (prep_frame_view_)
    return;

  FrameReference frame_ref(frame);

  int expected_page_count = 0;
  if (!CalculateNumberOfPages(frame, node, &expected_page_count)) {
    DidFinishPrinting(FAIL_PRINT_INIT);
    return;   
  }

  if (!expected_page_count) {
    DidFinishPrinting(FAIL_PRINT);
    return;
  }

  if (delegate_->IsAskPrintSettingsEnabled()) {
    blink::WebPrintScalingOption scaling_option =
        print_pages_params_->params.print_scaling_option;

    PrintMsg_PrintPages_Params print_settings;
    auto self = weak_ptr_factory_.GetWeakPtr();
    GetPrintSettingsFromUser(frame_ref.GetFrame(), node, expected_page_count,
                             is_scripted, &print_settings);
    if (!self)
      return;

    print_settings.params.print_scaling_option = scaling_option;
    SetPrintPagesParams(print_settings);
    if (!print_settings.params.dpi || !print_settings.params.document_cookie) {
      DidFinishPrinting(OK);   
      return;
    }
  }

  if (!RenderPagesForPrint(frame_ref.GetFrame(), node)) {
    LOG(ERROR) << "RenderPagesForPrint failed";
    DidFinishPrinting(FAIL_PRINT);
  }
  scripting_throttler_.Reset();
}
