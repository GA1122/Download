bool IsWebPrintScalingOptionFitToPage(const PrintMsg_Print_Params& params) {
  return params.print_scaling_option ==
         blink::kWebPrintScalingOptionFitToPrintableArea;
}
