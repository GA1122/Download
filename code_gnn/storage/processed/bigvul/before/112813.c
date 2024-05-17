void ChromeMockRenderThread::OnTempFileForPrintingWritten(int render_view_id,
                                                          int browser_fd) {
  close(browser_fd);
}
