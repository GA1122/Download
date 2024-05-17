bool BackendImpl::InitBackingStore(bool* file_created) {
  if (!base::CreateDirectory(path_))
    return false;

  base::FilePath index_name = path_.AppendASCII(kIndexName);

  int flags = base::File::FLAG_READ | base::File::FLAG_WRITE |
              base::File::FLAG_OPEN_ALWAYS | base::File::FLAG_EXCLUSIVE_WRITE;
  base::File base_file(index_name, flags);
  if (!base_file.IsValid())
    return false;

  bool ret = true;
  *file_created = base_file.created();

  scoped_refptr<disk_cache::File> file(
      new disk_cache::File(std::move(base_file)));
  if (*file_created)
    ret = CreateBackingStore(file.get());

  file = NULL;
  if (!ret)
    return false;

  index_ = new MappedFile();
  data_ = static_cast<Index*>(index_->Init(index_name, 0));
  if (!data_) {
    LOG(ERROR) << "Unable to map Index file";
    return false;
  }

  if (index_->GetLength() < sizeof(Index)) {
    LOG(ERROR) << "Corrupt Index file";
    return false;
  }

  return true;
}
