void WebMediaPlayerImpl::DoLoad(LoadType load_type,
                                const blink::WebURL& url,
                                CORSMode cors_mode) {
  TRACE_EVENT1("media", "WebMediaPlayerImpl::DoLoad", "id", media_log_->id());
  DVLOG(1) << __func__;
  DCHECK(main_task_runner_->BelongsToCurrentThread());

  GURL gurl(url);
  ReportMetrics(load_type, gurl, frame_->GetSecurityOrigin(), media_log_.get());

  if (load_type == kLoadTypeURL) {
    if (preload_ == MultibufferDataSource::METADATA) {
      UMA_HISTOGRAM_BOOLEAN("Media.SRC.PreloadMetaDataHasPoster", has_poster_);
    } else if (preload_ == MultibufferDataSource::AUTO) {
      UMA_HISTOGRAM_BOOLEAN("Media.SRC.PreloadAutoHasPoster", has_poster_);
    }
  }

  static base::debug::CrashKeyString* subresource_url =
      base::debug::AllocateCrashKeyString("subresource_url",
                                          base::debug::CrashKeySize::Size256);
  base::debug::SetCrashKeyString(subresource_url, gurl.spec());

  loaded_url_ = gurl;

  load_type_ = load_type;

  SetNetworkState(WebMediaPlayer::kNetworkStateLoading);
  SetReadyState(WebMediaPlayer::kReadyStateHaveNothing);
  media_log_->AddEvent(media_log_->CreateLoadEvent(url.GetString().Utf8()));
  load_start_time_ = base::TimeTicks::Now();

  media_metrics_provider_->Initialize(load_type == kLoadTypeMediaSource,
                                      frame_ == frame_->Top(),
                                      frame_->Top()->GetSecurityOrigin());

  if (load_type == kLoadTypeMediaSource) {
    StartPipeline();
  } else {
    data_source_.reset(new MultibufferDataSource(
        main_task_runner_,
        url_index_->GetByUrl(url, static_cast<UrlData::CORSMode>(cors_mode)),
        media_log_.get(), &buffered_data_source_host_,
        base::Bind(&WebMediaPlayerImpl::NotifyDownloading, AsWeakPtr())));
    data_source_->SetPreload(preload_);
    data_source_->SetIsClientAudioElement(client_->IsAudioElement());
    data_source_->Initialize(
        base::Bind(&WebMediaPlayerImpl::DataSourceInitialized, AsWeakPtr()));
  }

#if defined(OS_ANDROID)   
  cast_impl_.Initialize(url, frame_, delegate_id_);
#endif
}
