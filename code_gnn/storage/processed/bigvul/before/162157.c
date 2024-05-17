void RenderProcessHostImpl::UpdateProcessPriority() {
  if (!run_renderer_in_process() && (!child_process_launcher_.get() ||
                                     child_process_launcher_->IsStarting())) {
    priority_.background = kLaunchingProcessIsBackgrounded;
    priority_.boost_for_pending_views =
        kLaunchingProcessIsBoostedForPendingView;
    return;
  }

  const ChildProcessLauncherPriority priority = {
    visible_widgets_ == 0 && media_stream_count_ == 0 &&
        !base::CommandLine::ForCurrentProcess()->HasSwitch(
            switches::kDisableRendererBackgrounding),
    !!pending_views_,
#if defined(OS_ANDROID)
    ComputeEffectiveImportance(),
#endif
  };

  const bool should_background_changed =
      priority_.background != priority.background;
  if (priority_ == priority)
    return;

  TRACE_EVENT2("renderer_host", "RenderProcessHostImpl::UpdateProcessPriority",
               "should_background", priority.background, "has_pending_views",
               priority.boost_for_pending_views);
  priority_ = priority;

#if defined(OS_WIN)
  if (GetModuleHandle(L"cbstext.dll"))
    return;
#endif   

  if (!run_renderer_in_process()) {
    DCHECK(child_process_launcher_.get());
    DCHECK(!child_process_launcher_->IsStarting());
    child_process_launcher_->SetProcessPriority(priority_);
  }

  if (should_background_changed) {
    GetRendererInterface()->SetProcessBackgrounded(priority.background);
  }
}
