void MojoJpegDecodeAccelerator::OnInitializeDone(
    InitCB init_cb,
    JpegDecodeAccelerator::Client* client,
    bool success) {
  DCHECK(io_task_runner_->BelongsToCurrentThread());

  if (success)
    client_ = client;

  std::move(init_cb).Run(success);
}
