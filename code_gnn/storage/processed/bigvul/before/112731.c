void PrintingMessageFilter::OnTempFileForPrintingWritten(int render_view_id,
                                                         int sequence_number) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::FILE));
  SequenceToPathMap* map = &g_printing_file_descriptor_map.Get().map;
  SequenceToPathMap::iterator it = map->find(sequence_number);
  if (it == map->end()) {
    NOTREACHED() << "Got a sequence that we didn't pass to the "
                    "renderer: " << sequence_number;
    return;
  }
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&PrintingMessageFilter::CreatePrintDialogForFile,
                 this, render_view_id, it->second));

  map->erase(it);
}
