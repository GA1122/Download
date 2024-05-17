void DatabaseImpl::RenameIndex(int64_t transaction_id,
                               int64_t object_store_id,
                               int64_t index_id,
                               const base::string16& new_name) {
  idb_runner_->PostTask(
      FROM_HERE,
      base::Bind(&IDBThreadHelper::RenameIndex, base::Unretained(helper_),
                 transaction_id, object_store_id, index_id, new_name));
}
