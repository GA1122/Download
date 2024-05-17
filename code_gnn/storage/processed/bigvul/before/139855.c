Shell::Shell(ShellDelegate* delegate, base::SequencedWorkerPool* blocking_pool)
    : target_root_window_(nullptr),
      scoped_target_root_window_(nullptr),
      delegate_(delegate),
      window_positioner_(new WindowPositioner),
      activation_client_(nullptr),
#if defined(OS_CHROMEOS)
      display_configurator_(new ui::DisplayConfigurator()),
#endif   
      native_cursor_manager_(nullptr),
      simulate_modal_window_open_for_testing_(false),
      is_touch_hud_projection_enabled_(false),
      blocking_pool_(blocking_pool) {
  DCHECK(delegate_.get());
  DCHECK(aura::Env::GetInstanceDontCreate());
  gpu_support_.reset(delegate_->CreateGPUSupport());
  display_manager_.reset(new DisplayManager);
  window_tree_host_manager_.reset(new WindowTreeHostManager);
  user_metrics_recorder_.reset(new UserMetricsRecorder);

#if defined(OS_CHROMEOS)
  PowerStatus::Initialize();
#endif
}
