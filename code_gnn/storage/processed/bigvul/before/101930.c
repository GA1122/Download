void RenderParamsFromPrintSettings(const printing::PrintSettings& settings,
                                   PrintMsg_Print_Params* params) {
  params->page_size = settings.page_setup_device_units().physical_size();
  params->printable_size.SetSize(
      settings.page_setup_device_units().content_area().width(),
      settings.page_setup_device_units().content_area().height());
  params->margin_top = settings.page_setup_device_units().content_area().x();
  params->margin_left = settings.page_setup_device_units().content_area().y();
  params->dpi = settings.dpi();
  params->min_shrink = settings.min_shrink;
  params->max_shrink = settings.max_shrink;
  params->desired_dpi = settings.desired_dpi;
  params->document_cookie = 0;
  params->selection_only = settings.selection_only;
  params->supports_alpha_blend = settings.supports_alpha_blend();

  params->display_header_footer = settings.display_header_footer;
  if (!settings.display_header_footer)
    return;
  params->date = settings.date;
  params->title = settings.title;
  params->url = settings.url;
}
