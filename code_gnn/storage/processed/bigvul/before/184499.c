 PrintMsg_Print_Params::PrintMsg_Print_Params()
   : page_size(),
     content_size(),
     printable_area(),
     margin_top(0),
     margin_left(0),
     dpi(0),
     min_shrink(0),
     max_shrink(0),
     desired_dpi(0),
     document_cookie(0),
     selection_only(false),
     supports_alpha_blend(false),
      preview_ui_addr(),
      preview_request_id(0),
      is_first_request(false),
    fit_to_paper_size(true),
//     fit_to_paper_size(false),
      print_to_pdf(false),
      display_header_footer(false),
      date(),
     title(),
     url() {
 }