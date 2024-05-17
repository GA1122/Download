void MultibufferDataSource::OnRedirect(
    const scoped_refptr<UrlData>& destination) {
  if (!destination) {
    failed_ = true;
    if (init_cb_) {
      render_task_runner_->PostTask(
          FROM_HERE,
          base::Bind(&MultibufferDataSource::StartCallback, weak_ptr_));
    } else {
      base::AutoLock auto_lock(lock_);
      StopInternal_Locked();
    }
    StopLoader();
    return;
  }
  if (url_data()->url().GetOrigin() != destination->url().GetOrigin()) {
    single_origin_ = false;
  }
  SetReader(nullptr);
  url_data_and_loading_state_.SetUrlData(std::move(destination));

  if (url_data()) {
    url_data()->OnRedirect(
        base::Bind(&MultibufferDataSource::OnRedirect, weak_ptr_));

    if (init_cb_) {
      CreateResourceLoader(0, kPositionNotSpecified);
      if (reader_->Available()) {
        render_task_runner_->PostTask(
            FROM_HERE,
            base::Bind(&MultibufferDataSource::StartCallback, weak_ptr_));
      } else {
        reader_->Wait(
            1, base::Bind(&MultibufferDataSource::StartCallback, weak_ptr_));
      }
    } else if (read_op_) {
      CreateResourceLoader(read_op_->position(), kPositionNotSpecified);
      if (reader_->Available()) {
        render_task_runner_->PostTask(
            FROM_HERE,
            base::BindOnce(&MultibufferDataSource::ReadTask, weak_ptr_));
      } else {
        reader_->Wait(1,
                      base::Bind(&MultibufferDataSource::ReadTask, weak_ptr_));
      }
    }
  }
}
