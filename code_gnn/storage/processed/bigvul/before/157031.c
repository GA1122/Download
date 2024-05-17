void MultibufferDataSource::Initialize(const InitializeCB& init_cb) {
  DCHECK(render_task_runner_->BelongsToCurrentThread());
  DCHECK(init_cb);
  DCHECK(!reader_.get());

  init_cb_ = init_cb;

  url_data_and_loading_state_.SetLoadingState(
      UrlData::UrlDataWithLoadingState::LoadingState::kPreload);
  CreateResourceLoader(0, kPositionNotSpecified);

  if (reader_->Available()) {
    render_task_runner_->PostTask(
        FROM_HERE,
        base::Bind(&MultibufferDataSource::StartCallback, weak_ptr_));

    render_task_runner_->PostTask(
        FROM_HERE, base::BindOnce(&MultibufferDataSource::UpdateProgress,
                                  weak_factory_.GetWeakPtr()));
  } else {
    reader_->Wait(1,
                  base::Bind(&MultibufferDataSource::StartCallback, weak_ptr_));
  }
}
