bool PageLayoutIsEqual(const PrintMsg_PrintPages_Params& oldParams,
                       const PrintMsg_PrintPages_Params& newParams) {
  return oldParams.params.content_size == newParams.params.content_size &&
         oldParams.params.printable_area == newParams.params.printable_area &&
         oldParams.params.page_size == newParams.params.page_size &&
         oldParams.params.margin_top == newParams.params.margin_top &&
         oldParams.params.margin_left == newParams.params.margin_left &&
         oldParams.params.desired_dpi == newParams.params.desired_dpi &&
         oldParams.params.dpi == newParams.params.dpi;
}
