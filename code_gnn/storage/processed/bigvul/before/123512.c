FileStreamReader* BlobURLRequestJob::GetFileStreamReader(size_t index) {
  DCHECK_LT(index, blob_data_->items().size());
  const BlobData::Item& item = blob_data_->items().at(index);
  if (!IsFileType(item.type()))
    return NULL;
  if (index_to_reader_.find(index) == index_to_reader_.end())
    CreateFileStreamReader(index, 0);
  DCHECK(index_to_reader_[index]);
  return index_to_reader_[index];
}
