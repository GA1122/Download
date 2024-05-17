void PrintToPdfCallbackWithCheck(Metafile* metafile,
                                 gdata::DriveFileError error,
                                 const FilePath& path) {
  if (error != gdata::DRIVE_FILE_OK) {
    LOG(ERROR) << "Save to pdf failed to write: " << error;
  } else {
    metafile->SaveTo(path);
  }
  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      base::Bind(&base::DeletePointer<Metafile>, metafile));
}
