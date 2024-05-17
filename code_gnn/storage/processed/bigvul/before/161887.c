void ComputeWebKitPrintParamsInDesiredDpi(
    const PrintMsg_Print_Params& print_params,
    blink::WebPrintParams* webkit_print_params) {
  int dpi = GetDPI(&print_params);
  webkit_print_params->printer_dpi = dpi;
  webkit_print_params->rasterize_pdf = print_params.rasterize_pdf;
  webkit_print_params->print_scaling_option = print_params.print_scaling_option;

  webkit_print_params->print_content_area.width =
      ConvertUnit(print_params.content_size.width(), dpi, kPointsPerInch);
  webkit_print_params->print_content_area.height =
      ConvertUnit(print_params.content_size.height(), dpi, kPointsPerInch);

  webkit_print_params->printable_area.x =
      ConvertUnit(print_params.printable_area.x(), dpi, kPointsPerInch);
  webkit_print_params->printable_area.y =
      ConvertUnit(print_params.printable_area.y(), dpi, kPointsPerInch);
  webkit_print_params->printable_area.width =
      ConvertUnit(print_params.printable_area.width(), dpi, kPointsPerInch);
  webkit_print_params->printable_area.height =
      ConvertUnit(print_params.printable_area.height(), dpi, kPointsPerInch);

  webkit_print_params->paper_size.width =
      ConvertUnit(print_params.page_size.width(), dpi, kPointsPerInch);
  webkit_print_params->paper_size.height =
      ConvertUnit(print_params.page_size.height(), dpi, kPointsPerInch);
}
