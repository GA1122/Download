void PrintingMessageFilter::OnTempFileForPrintingWritten(int sequence_number) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  SequenceToPathMap* map = &g_printing_file_descriptor_map.Get().map;
  SequenceToPathMap::iterator it = map->find(sequence_number);
  if (it == map->end()) {
    NOTREACHED() << "Got a sequence that we didn't pass to the "
                    "renderer: " << sequence_number;
    return;
  }

  if (cloud_print_enabled_)
    print_dialog_cloud::CreatePrintDialogForFile(
        it->second,
        string16(),
        string16(),
        std::string("application/pdf"),
        true,
        false);
  else
    NOTIMPLEMENTED();

  map->erase(it);
}
