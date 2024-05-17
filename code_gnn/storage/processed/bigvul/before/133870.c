bool HFSIterator::ReadCatalogFile() {
  catalog_file_.reset(new HFSForkReadStream(this, volume_header_.catalogFile));
  catalog_.reset(new HFSBTreeIterator());
  return catalog_->Init(catalog_file_.get());
}
