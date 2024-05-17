void ResourceDispatcherHostImpl::UpdateLoadStateOnUI(
    LoaderDelegate* loader_delegate, std::unique_ptr<LoadInfoList> infos) {
  DCHECK(Get()->main_thread_task_runner_->BelongsToCurrentThread());

  std::unique_ptr<LoadInfoMap> info_map =
      PickMoreInterestingLoadInfos(std::move(infos));
  for (const auto& load_info : *info_map) {
    loader_delegate->LoadStateChanged(
        load_info.first, load_info.second.host, load_info.second.load_state,
        load_info.second.upload_position, load_info.second.upload_size);
  }
}
