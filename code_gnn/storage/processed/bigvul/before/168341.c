void BrowserView::ProcessFullscreen(bool fullscreen,
                                    const GURL& url,
                                    ExclusiveAccessBubbleType bubble_type) {
  if (in_process_fullscreen_)
    return;
  in_process_fullscreen_ = true;

  if (fullscreen) {
    views::FocusManager* focus_manager = GetFocusManager();
    DCHECK(focus_manager);
    if (GetLocationBarView()->Contains(focus_manager->GetFocusedView()))
      focus_manager->ClearFocus();

    if (FullscreenControlHost::IsFullscreenExitUIEnabled()) {
#if defined(USE_AURA)
      frame_->GetNativeView()->AddPreTargetHandler(
          GetFullscreenControlHost(), ui::EventTarget::Priority::kSystem);
#else
      fullscreen_control_host_event_monitor_ =
          views::EventMonitor::CreateWindowMonitor(GetFullscreenControlHost(),
                                                   GetNativeWindow());
#endif
    }
  } else {
    exclusive_access_bubble_.reset();

    if (fullscreen_control_host_) {
      fullscreen_control_host_->Hide(false);
#if defined(USE_AURA)
      auto* native_view = frame_->GetNativeView();
      if (native_view)
        native_view->RemovePreTargetHandler(fullscreen_control_host_.get());
#else
      fullscreen_control_host_event_monitor_.reset();
#endif
    }
  }

  frame_->SetFullscreen(fullscreen);

  const bool should_stay_in_immersive =
      !fullscreen &&
      immersive_mode_controller_->ShouldStayImmersiveAfterExitingFullscreen();
  if (ShouldUseImmersiveFullscreenForUrl(url) && !should_stay_in_immersive)
    immersive_mode_controller_->SetEnabled(fullscreen);

  browser_->WindowFullscreenStateWillChange();
  browser_->WindowFullscreenStateChanged();

  if (fullscreen && !chrome::IsRunningInAppMode()) {
    UpdateExclusiveAccessExitBubbleContent(url, bubble_type,
                                           ExclusiveAccessBubbleHideCallback(),
                                            false);
  }

  in_process_fullscreen_ = false;
  ToolbarSizeChanged(false);

  WebContents* contents = browser_->tab_strip_model()->GetActiveWebContents();
  if (contents && PermissionRequestManager::FromWebContents(contents))
    PermissionRequestManager::FromWebContents(contents)->UpdateAnchorPosition();
}
