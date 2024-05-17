WebMediaPlayerMS::WebMediaPlayerMS(
    blink::WebLocalFrame* frame,
    blink::WebMediaPlayerClient* client,
    media::WebMediaPlayerDelegate* delegate,
    std::unique_ptr<media::MediaLog> media_log,
    std::unique_ptr<MediaStreamRendererFactory> factory,
    scoped_refptr<base::SingleThreadTaskRunner> io_task_runner,
    scoped_refptr<base::SingleThreadTaskRunner> compositor_task_runner,
    scoped_refptr<base::SingleThreadTaskRunner> media_task_runner,
    scoped_refptr<base::TaskRunner> worker_task_runner,
    media::GpuVideoAcceleratorFactories* gpu_factories,
    const blink::WebString& sink_id,
    CreateSurfaceLayerBridgeCB create_bridge_callback,
    base::RepeatingCallback<std::unique_ptr<blink::WebVideoFrameSubmitter>()>
        create_submitter_callback,
    bool surface_layer_for_video_enabled)
    : frame_(frame),
      network_state_(WebMediaPlayer::kNetworkStateEmpty),
      ready_state_(WebMediaPlayer::kReadyStateHaveNothing),
      buffered_(static_cast<size_t>(0)),
      client_(client),
      delegate_(delegate),
      delegate_id_(0),
      paused_(true),
      video_rotation_(media::VIDEO_ROTATION_0),
      media_log_(std::move(media_log)),
      renderer_factory_(std::move(factory)),
      io_task_runner_(io_task_runner),
      compositor_task_runner_(compositor_task_runner),
      media_task_runner_(media_task_runner),
      worker_task_runner_(worker_task_runner),
      gpu_factories_(gpu_factories),
      initial_audio_output_device_id_(sink_id.Utf8()),
      volume_(1.0),
      volume_multiplier_(1.0),
      should_play_upon_shown_(false),
      create_bridge_callback_(std::move(create_bridge_callback)),
      create_submitter_callback_(create_submitter_callback),
      surface_layer_for_video_enabled_(surface_layer_for_video_enabled) {
  DVLOG(1) << __func__;
  DCHECK(client);
  DCHECK(delegate_);
  delegate_id_ = delegate_->AddObserver(this);

  media_log_->AddEvent(
      media_log_->CreateEvent(media::MediaLogEvent::WEBMEDIAPLAYER_CREATED));
}
