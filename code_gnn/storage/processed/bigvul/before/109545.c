void PrepareFrameAndViewForPrint::UpdatePrintParams(
    const PrintMsg_Print_Params& print_params) {
  DCHECK(!finished_);
  WebKit::WebPrintParams webkit_print_params;
  ComputeWebKitPrintParamsInDesiredDpi(print_params, &webkit_print_params);

  if (webkit_print_params.printContentArea ==
          web_print_params_.printContentArea &&
      webkit_print_params.printableArea == web_print_params_.printableArea &&
      webkit_print_params.paperSize == web_print_params_.paperSize &&
      webkit_print_params.printScalingOption ==
          web_print_params_.printScalingOption) {
    return;
  }

  frame_->printEnd();
  StartPrinting(webkit_print_params);
}
