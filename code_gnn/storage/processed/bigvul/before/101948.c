void MockPrinter::SetPrintedPagesCount(int cookie, int number_pages) {
  EXPECT_EQ(document_cookie_, cookie);
  EXPECT_EQ(PRINTER_PRINTING, printer_status_);
  EXPECT_EQ(0, number_pages_);
  EXPECT_EQ(0, page_number_);

  number_pages_ = number_pages;
  page_number_ = 0;
  pages_.clear();
}
