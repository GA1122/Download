bool GlobalHistogramAllocator::CreateSpareFile(const FilePath& spare_path,
                                               size_t size) {
  FilePath temp_spare_path = spare_path.AddExtension(FILE_PATH_LITERAL(".tmp"));
  bool success = true;
  {
    File spare_file(temp_spare_path, File::FLAG_CREATE_ALWAYS |
                                         File::FLAG_READ | File::FLAG_WRITE);
    if (!spare_file.IsValid())
      return false;

    MemoryMappedFile mmfile;
    mmfile.Initialize(std::move(spare_file), {0, size},
                      MemoryMappedFile::READ_WRITE_EXTEND);
    success = mmfile.IsValid();
  }

  if (success)
    success = ReplaceFile(temp_spare_path, spare_path, nullptr);

  if (!success)
    DeleteFile(temp_spare_path,  false);

  return success;
}
