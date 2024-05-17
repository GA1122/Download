void PrintViewManagerBase::OnDidGetPrintedPagesCount(int cookie,
                                                     int number_pages) {
  PrintManager::OnDidGetPrintedPagesCount(cookie, number_pages);
   OpportunisticallyCreatePrintJob(cookie);
 }
