 void PrintMsg_Print_Params::Reset() {
   page_size = gfx::Size();
   content_size = gfx::Size();
   printable_area = gfx::Rect();
   margin_top = 0;
   margin_left = 0;
   dpi = 0;
   scale_factor = 1.0f;
   rasterize_pdf = false;
   document_cookie = 0;
   selection_only = false;
   supports_alpha_blend = false;
   preview_ui_id = -1;
   preview_request_id = 0;
   is_first_request = false;
   print_scaling_option = blink::kWebPrintScalingOptionSourceSize;
   print_to_pdf = false;
    display_header_footer = false;
    title = base::string16();
    url = base::string16();
//   header_template = base::string16();
//   footer_template = base::string16();
    should_print_backgrounds = false;
    printed_doc_type = printing::SkiaDocumentType::PDF;
  }