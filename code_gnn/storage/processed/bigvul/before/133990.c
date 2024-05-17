void AppListSyncableService::SetOemFolderName(const std::string& name) {
  oem_folder_name_ = name;
  AppListFolderItem* oem_folder = model_->FindFolderItem(kOemFolderId);
  if (oem_folder)
    model_->SetItemName(oem_folder, oem_folder_name_);
}
