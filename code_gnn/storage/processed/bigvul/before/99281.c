void ResourceMessageFilter::OnClipboardWriteObjectsSync(
    const Clipboard::ObjectMap& objects,
    base::SharedMemoryHandle bitmap_handle) {
  DCHECK(base::SharedMemory::IsHandleValid(bitmap_handle))
      << "Bad bitmap handle";
  Clipboard::ObjectMap* long_living_objects = new Clipboard::ObjectMap(objects);

  Clipboard::ReplaceSharedMemHandle(long_living_objects, bitmap_handle,
                                    handle());

  ChromeThread::PostTask(
      ChromeThread::UI,
      FROM_HERE,
      new WriteClipboardTask(long_living_objects));
}
