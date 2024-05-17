void MockPrinter::PrintPage(const PrintHostMsg_DidPrintPage_Params& params) {
  EXPECT_EQ(PRINTER_PRINTING, printer_status_);
  EXPECT_EQ(document_cookie_, params.document_cookie);
  EXPECT_EQ(page_number_, params.page_number);
  EXPECT_LE(params.page_number, number_pages_);

#if defined(OS_WIN) || defined(OS_MACOSX)
  EXPECT_GT(params.data_size, 0U);
#if defined(OS_WIN)
  base::SharedMemory metafile_data(params.metafile_data_handle, true,
                                   GetCurrentProcess());
#elif defined(OS_MACOSX)
  base::SharedMemory metafile_data(params.metafile_data_handle, true);
#endif
  metafile_data.Map(params.data_size);
#if defined(OS_MACOSX)
  printing::PdfMetafileCg metafile;
#else
  printing::NativeMetafile metafile;
#endif
  metafile.InitFromData(metafile_data.memory(), params.data_size);
  printing::Image image(metafile);
  MockPrinterPage* page_data = new MockPrinterPage(metafile_data.memory(),
                                                   params.data_size,
                                                   image);
  if (!page_data) {
    printer_status_ = PRINTER_ERROR;
    return;
  }

  scoped_refptr<MockPrinterPage> page(page_data);
  pages_.push_back(page);
#endif

  ++page_number_;
  if (number_pages_ == page_number_)
    ResetPrinter();
}
