void PluginInstance::NPP_Destroy() {
  DCHECK(npp_functions_ != 0);
  DCHECK(npp_functions_->destroy != 0);

  if (npp_functions_->destroy != 0) {
    NPSavedData *savedData = 0;
    npp_functions_->destroy(npp_, &savedData);

    DCHECK(savedData == 0);
  }

  for (unsigned int file_index = 0; file_index < files_created_.size();
       file_index++) {
    file_util::Delete(files_created_[file_index], false);
  }

  timers_.clear();
}
