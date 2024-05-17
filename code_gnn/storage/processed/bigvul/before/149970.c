LayerTreeHostImpl::LayerTreeHostImpl(
    const LayerTreeSettings& settings,
    LayerTreeHostImplClient* client,
    TaskRunnerProvider* task_runner_provider,
    RenderingStatsInstrumentation* rendering_stats_instrumentation,
    TaskGraphRunner* task_graph_runner,
    std::unique_ptr<MutatorHost> mutator_host,
    int id,
    scoped_refptr<base::SequencedTaskRunner> image_worker_task_runner)
    : client_(client),
      task_runner_provider_(task_runner_provider),
      current_begin_frame_tracker_(BEGINFRAMETRACKER_FROM_HERE),
      compositor_frame_sink_(nullptr),
      need_update_gpu_rasterization_status_(false),
      content_is_suitable_for_gpu_rasterization_(true),
      has_gpu_rasterization_trigger_(false),
      use_gpu_rasterization_(false),
      use_msaa_(false),
      gpu_rasterization_status_(GpuRasterizationStatus::OFF_DEVICE),
      input_handler_client_(NULL),
      did_lock_scrolling_layer_(false),
      wheel_scrolling_(false),
      scroll_affects_scroll_handler_(false),
      scroll_layer_id_mouse_currently_over_(Layer::INVALID_ID),
      scroll_layer_id_mouse_currently_captured_(Layer::INVALID_ID),
      tile_priorities_dirty_(false),
      settings_(settings),
      visible_(false),
      cached_managed_memory_policy_(settings.gpu_memory_policy),
      is_synchronous_single_threaded_(!task_runner_provider->HasImplThread() &&
                                      !settings.single_thread_proxy_scheduler),
      tile_manager_(this,
                    GetTaskRunner(),
                    std::move(image_worker_task_runner),
                    is_synchronous_single_threaded_
                        ? std::numeric_limits<size_t>::max()
                        : settings.scheduled_raster_task_limit,
                    settings.ToTileManagerSettings()),
      pinch_gesture_active_(false),
      pinch_gesture_end_should_clear_scrolling_node_(false),
      fps_counter_(
          FrameRateCounter::Create(task_runner_provider_->HasImplThread())),
      memory_history_(MemoryHistory::Create()),
      debug_rect_history_(DebugRectHistory::Create()),
      max_memory_needed_bytes_(0),
      resourceless_software_draw_(false),
      mutator_host_(std::move(mutator_host)),
      rendering_stats_instrumentation_(rendering_stats_instrumentation),
      micro_benchmark_controller_(this),
      task_graph_runner_(task_graph_runner),
      id_(id),
      requires_high_res_to_draw_(false),
      is_likely_to_require_a_draw_(false),
      has_valid_compositor_frame_sink_(false),
      mutator_(nullptr) {
  DCHECK(mutator_host_);
  mutator_host_->SetMutatorHostClient(this);

  DCHECK(task_runner_provider_->IsImplThread());
  DidVisibilityChange(this, visible_);

  SetDebugState(settings.initial_debug_state);

  active_tree_ = base::MakeUnique<LayerTreeImpl>(
      this, new SyncedProperty<ScaleGroup>, new SyncedBrowserControls,
      new SyncedElasticOverscroll);
  active_tree_->property_trees()->is_active = true;

  viewport_ = Viewport::Create(this);

  TRACE_EVENT_OBJECT_CREATED_WITH_ID(TRACE_DISABLED_BY_DEFAULT("cc.debug"),
                                     "cc::LayerTreeHostImpl", id_);

  browser_controls_offset_manager_ = BrowserControlsOffsetManager::Create(
      this, settings.top_controls_show_threshold,
      settings.top_controls_hide_threshold);

  tile_manager_.SetDecodedImageTracker(&decoded_image_tracker_);
}
