bool GlobalHistogramAllocator::CreateWithFile(
    const FilePath& file_path,
    size_t size,
    uint64_t id,
    StringPiece name) {
  bool exists = PathExists(file_path);
  File file(
      file_path, File::FLAG_OPEN_ALWAYS | File::FLAG_SHARE_DELETE |
                 File::FLAG_READ | File::FLAG_WRITE);

  std::unique_ptr<MemoryMappedFile> mmfile(new MemoryMappedFile());
  if (exists) {
    size = saturated_cast<size_t>(file.GetLength());
    mmfile->Initialize(std::move(file), MemoryMappedFile::READ_WRITE);
  } else {
    mmfile->Initialize(std::move(file), {0, size},
                       MemoryMappedFile::READ_WRITE_EXTEND);
  }
  if (!mmfile->IsValid() ||
      !FilePersistentMemoryAllocator::IsFileAcceptable(*mmfile, true)) {
    NOTREACHED() << file_path;
    return false;
  }

  Set(WrapUnique(new GlobalHistogramAllocator(
      std::make_unique<FilePersistentMemoryAllocator>(std::move(mmfile), size,
                                                      id, name, false))));
  Get()->SetPersistentLocation(file_path);
  return true;
}
