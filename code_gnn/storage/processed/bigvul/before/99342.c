void RenderParamsFromPrintSettings(const printing::PrintSettings& settings,
                                   ViewMsg_Print_Params* params) {
  DCHECK(params);
#if defined(OS_WIN) || defined(OS_MACOSX)
  params->printable_size.SetSize(
      settings.page_setup_pixels().content_area().width(),
      settings.page_setup_pixels().content_area().height());
  params->dpi = settings.dpi();
  params->min_shrink = settings.min_shrink;
  params->max_shrink = settings.max_shrink;
  params->desired_dpi = settings.desired_dpi;
  params->document_cookie = 0;
  params->selection_only = settings.selection_only;
#else
  NOTIMPLEMENTED();
#endif
}
