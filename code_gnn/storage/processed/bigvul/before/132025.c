void WriteObjectsOnUIThread(ui::Clipboard::ObjectMap* objects) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  static ui::Clipboard* clipboard = ui::Clipboard::GetForCurrentThread();
   clipboard->WriteObjects(ui::CLIPBOARD_TYPE_COPY_PASTE, *objects);
 }
