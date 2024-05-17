bool PrintMsg_Print_Params_IsValid(const PrintMsg_Print_Params& params) {
  return !params.content_size.IsEmpty() && !params.page_size.IsEmpty() &&
         !params.printable_area.IsEmpty() && params.document_cookie &&
         params.desired_dpi && params.max_shrink && params.min_shrink &&
         params.dpi && (params.margin_top >= 0) && (params.margin_left >= 0);
}