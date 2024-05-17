void MultibufferDataSource::SetBitrate(int bitrate) {
  render_task_runner_->PostTask(
      FROM_HERE, base::BindOnce(&MultibufferDataSource::SetBitrateTask,
                                weak_factory_.GetWeakPtr(), bitrate));
}
