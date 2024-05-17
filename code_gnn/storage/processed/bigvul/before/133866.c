bool HFSIterator::Next() {
  if (!catalog_->HasNext())
    return false;

  bool keep_going = false;
  do {
    keep_going = catalog_->Next();
    if (keep_going) {
      if (!catalog_->current_record()->unexported &&
          (catalog_->current_record()->record_type == kHFSPlusFolderRecord ||
           catalog_->current_record()->record_type == kHFSPlusFileRecord)) {
        return true;
      }
      keep_going = catalog_->HasNext();
    }
  } while (keep_going);

  return keep_going;
}
