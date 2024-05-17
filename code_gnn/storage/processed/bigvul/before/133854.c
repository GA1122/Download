base::string16 HFSIterator::GetPath() {
  return catalog_->current_record()->path;
}
