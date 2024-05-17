void HeadlessPrintManager::OnPrintingFailed(int cookie) {
  ReleaseJob(PRINTING_FAILED);
}
