void BlobURLRequestJob::DeleteCurrentFileReader() {
  IndexToReaderMap::iterator found = index_to_reader_.find(current_item_index_);
  if (found != index_to_reader_.end() && found->second) {
    delete found->second;
    index_to_reader_.erase(found);
  }
}
