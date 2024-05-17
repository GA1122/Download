void PrintToPdfCallback(Metafile* metafile, const FilePath& path) {
  metafile->SaveTo(path);
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&base::DeletePointer<Metafile>, metafile));
}
