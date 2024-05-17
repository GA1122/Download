bool PrintMsg_Print_Params_IsValid(const PrintMsg_Print_Params& params) {
  return !params.content_size.IsEmpty() && !params.page_size.IsEmpty() &&
         !params.printable_area.IsEmpty() && params.document_cookie &&
         params.dpi && params.margin_top >= 0 && params.margin_left >= 0 &&
         params.dpi > kMinDpi && params.document_cookie != 0;
}
