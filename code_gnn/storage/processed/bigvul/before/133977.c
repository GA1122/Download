syncer::SyncMergeResult AppListSyncableService::MergeDataAndStartSyncing(
    syncer::ModelType type,
    const syncer::SyncDataList& initial_sync_data,
    scoped_ptr<syncer::SyncChangeProcessor> sync_processor,
    scoped_ptr<syncer::SyncErrorFactory> error_handler) {
  DCHECK(!sync_processor_.get());
  DCHECK(sync_processor.get());
  DCHECK(error_handler.get());

  GetModel();

  sync_processor_ = sync_processor.Pass();
  sync_error_handler_ = error_handler.Pass();
  if (switches::IsFolderUIEnabled())
    model_->SetFoldersEnabled(true);

  syncer::SyncMergeResult result = syncer::SyncMergeResult(type);
  result.set_num_items_before_association(sync_items_.size());
  VLOG(1) << this << ": MergeDataAndStartSyncing: "
          << initial_sync_data.size();

  std::set<std::string> unsynced_items;
  for (SyncItemMap::const_iterator iter = sync_items_.begin();
       iter != sync_items_.end(); ++iter) {
    unsynced_items.insert(iter->first);
  }

  size_t new_items = 0, updated_items = 0;
  for (syncer::SyncDataList::const_iterator iter = initial_sync_data.begin();
       iter != initial_sync_data.end(); ++iter) {
    const syncer::SyncData& data = *iter;
    const std::string& item_id = data.GetSpecifics().app_list().item_id();
    const sync_pb::AppListSpecifics& specifics = data.GetSpecifics().app_list();
    DVLOG(2) << this << "  Initial Sync Item: " << item_id
             << " Type: " << specifics.item_type();
    DCHECK_EQ(syncer::APP_LIST, data.GetDataType());
    if (ProcessSyncItemSpecifics(specifics))
      ++new_items;
    else
      ++updated_items;
    if (specifics.item_type() != sync_pb::AppListSpecifics::TYPE_FOLDER &&
        !IsUnRemovableDefaultApp(item_id) &&
        !AppIsOem(item_id) &&
        !AppIsDefault(extension_system_->extension_service(), item_id)) {
      VLOG(2) << "Syncing non-default item: " << item_id;
      first_app_list_sync_ = false;
    }
    unsynced_items.erase(item_id);
  }
  result.set_num_items_after_association(sync_items_.size());
  result.set_num_items_added(new_items);
  result.set_num_items_deleted(0);
  result.set_num_items_modified(updated_items);

  initial_sync_data_processed_ = true;

  syncer::SyncChangeList change_list;
  for (std::set<std::string>::iterator iter = unsynced_items.begin();
       iter != unsynced_items.end(); ++iter) {
    SyncItem* sync_item = FindSyncItem(*iter);
    if (!sync_item)
      continue;
    VLOG(2) << this << " -> SYNC ADD: " << sync_item->ToString();
    change_list.push_back(SyncChange(FROM_HERE,  SyncChange::ACTION_ADD,
                                     GetSyncDataFromSyncItem(sync_item)));
  }
  sync_processor_->ProcessSyncChanges(FROM_HERE, change_list);

  ResolveFolderPositions();

  model_observer_.reset(new ModelObserver(this));

  return result;
}
