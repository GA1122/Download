void ResourceMessageFilter::OnTempFileForPrintingWritten(int fd_in_browser) {
  FdMap* map = &Singleton<PrintingFileDescriptorMap>::get()->map;
  FdMap::iterator it = map->find(fd_in_browser);
  if (it == map->end()) {
    NOTREACHED() << "Got a file descriptor that we didn't pass to the "
                    "renderer: " << fd_in_browser;
    return;
  }

#if defined(TOOLKIT_GTK)
  PrintDialogGtk::CreatePrintDialogForPdf(it->second);
#else
  if (cloud_print_enabled_)
    PrintDialogCloud::CreatePrintDialogForPdf(it->second);
  else
    NOTIMPLEMENTED();
#endif

  map->erase(it);
}
