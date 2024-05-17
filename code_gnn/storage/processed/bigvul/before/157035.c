MultibufferDataSource::MultibufferDataSource(
    const scoped_refptr<base::SingleThreadTaskRunner>& task_runner,
    scoped_refptr<UrlData> url_data_arg,
    MediaLog* media_log,
    BufferedDataSourceHost* host,
    const DownloadingCB& downloading_cb)
    : total_bytes_(kPositionNotSpecified),
      streaming_(false),
      loading_(false),
      failed_(false),
      render_task_runner_(task_runner),
      stop_signal_received_(false),
      media_has_played_(false),
      single_origin_(true),
      cancel_on_defer_(false),
      preload_(AUTO),
      bitrate_(0),
      playback_rate_(0.0),
      media_log_(media_log),
      host_(host),
      downloading_cb_(downloading_cb),
      weak_factory_(this) {
  url_data_and_loading_state_.SetUrlData(std::move(url_data_arg));
  weak_ptr_ = weak_factory_.GetWeakPtr();
  DCHECK(host_);
  DCHECK(downloading_cb_);
  DCHECK(render_task_runner_->BelongsToCurrentThread());
  DCHECK(url_data());
  url_data()->Use();
  url_data()->OnRedirect(
      base::Bind(&MultibufferDataSource::OnRedirect, weak_ptr_));
}
