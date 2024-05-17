void BlobURLRequestJob::CreateFileStreamReader(size_t index,
                                               int64 additional_offset) {
  DCHECK_LT(index, blob_data_->items().size());
  const BlobData::Item& item = blob_data_->items().at(index);
  DCHECK(IsFileType(item.type()));
  DCHECK_EQ(0U, index_to_reader_.count(index));

  FileStreamReader* reader = NULL;
  switch (item.type()) {
    case BlobData::Item::TYPE_FILE:
      reader = new LocalFileStreamReader(
          file_thread_proxy_,
          item.path(),
          item.offset() + additional_offset,
          item.expected_modification_time());
      break;
    case BlobData::Item::TYPE_FILE_FILESYSTEM:
      reader = file_system_context_->CreateFileStreamReader(
          fileapi::FileSystemURL(file_system_context_->CrackURL(item.url())),
          item.offset() + additional_offset,
          item.expected_modification_time());
      break;
    default:
      NOTREACHED();
  }
  DCHECK(reader);
  index_to_reader_[index] = reader;
}
