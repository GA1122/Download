void ResourceMessageFilter::OnClipboardWriteObjectsAsync(
    const Clipboard::ObjectMap& objects) {
  Clipboard::ObjectMap* long_living_objects = new Clipboard::ObjectMap(objects);

  long_living_objects->erase(Clipboard::CBF_SMBITMAP);

  ChromeThread::PostTask(
      ChromeThread::UI,
      FROM_HERE,
      new WriteClipboardTask(long_living_objects));
}
