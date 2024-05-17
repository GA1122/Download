void ResourceDispatcherHostImpl::UpdateLoadInfo() {
  std::unique_ptr<LoadInfoList> infos(GetInterestingPerFrameLoadInfos());

  DCHECK(!waiting_on_load_state_ack_);
  waiting_on_load_state_ack_ = true;
  main_thread_task_runner_->PostTaskAndReply(
      FROM_HERE,
      base::BindOnce(UpdateLoadStateOnUI, loader_delegate_, std::move(infos)),
      base::BindOnce(&ResourceDispatcherHostImpl::AckUpdateLoadInfo,
                     weak_factory_on_io_.GetWeakPtr()));
}
