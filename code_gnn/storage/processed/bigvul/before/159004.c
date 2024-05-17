pp::Buffer_Dev PDFiumEngine::PrintPagesAsPDF(
    const PP_PrintPageNumberRange_Dev* page_ranges,
    uint32_t page_range_count,
    const PP_PrintSettings_Dev& print_settings) {
  if (!page_range_count)
    return pp::Buffer_Dev();

  DCHECK(doc_);
  FPDF_DOCUMENT output_doc = FPDF_CreateNewDocument();
  if (!output_doc)
    return pp::Buffer_Dev();

  SaveSelectedFormForPrint();

  std::string page_number_str;
  for (uint32_t index = 0; index < page_range_count; ++index) {
    if (!page_number_str.empty())
      page_number_str.append(",");
    const PP_PrintPageNumberRange_Dev& range = page_ranges[index];
    page_number_str.append(base::UintToString(range.first_page_number + 1));
    if (range.first_page_number != range.last_page_number) {
      page_number_str.append("-");
      page_number_str.append(base::UintToString(range.last_page_number + 1));
    }
  }

  std::vector<uint32_t> page_numbers =
      GetPageNumbersFromPrintPageNumberRange(page_ranges, page_range_count);
  for (uint32_t page_number : page_numbers) {
    pages_[page_number]->GetPage();
    if (!IsPageVisible(page_number))
      pages_[page_number]->Unload();
  }

  FPDF_CopyViewerPreferences(output_doc, doc_);
  if (!FPDF_ImportPages(output_doc, doc_, page_number_str.c_str(), 0)) {
    FPDF_CloseDocument(output_doc);
    return pp::Buffer_Dev();
  }

  FitContentsToPrintableAreaIfRequired(output_doc, print_settings);

  pp::Buffer_Dev buffer = GetFlattenedPrintData(output_doc);
  FPDF_CloseDocument(output_doc);
  return buffer;
}
