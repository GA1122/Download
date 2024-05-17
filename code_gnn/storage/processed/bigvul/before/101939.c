MockPrinter::MockPrinter()
  : dpi_(printing::kPointsPerInch),
    max_shrink_(2.0),
    min_shrink_(1.25),
    desired_dpi_(printing::kPointsPerInch),
    selection_only_(false),
    document_cookie_(-1),
    current_document_cookie_(0),
    printer_status_(PRINTER_READY),
    number_pages_(0),
    page_number_(0),
    is_first_request_(true),
    preview_request_id_(0),
    display_header_footer_(false),
    date_(ASCIIToUTF16("date")),
    title_(ASCIIToUTF16("title")),
    url_(ASCIIToUTF16("url")),
    use_invalid_settings_(false) {
  page_size_.SetSize(static_cast<int>(8.5 * dpi_),
                     static_cast<int>(11.0 * dpi_));
  printable_size_.SetSize(static_cast<int>((7.5 * dpi_)),
                          static_cast<int>((10.0 * dpi_)));
  margin_left_ = margin_top_ = static_cast<int>(0.5 * dpi_);
}
