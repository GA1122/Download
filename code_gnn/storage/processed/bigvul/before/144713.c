WebContentsImpl::~WebContentsImpl() {
  is_being_destroyed_ = true;

  rwh_input_event_router_.reset();

  for (FrameTreeNode* node : frame_tree_.Nodes()) {
    node->render_manager()->ClearRFHsPendingShutdown();
    node->render_manager()->ClearWebUIInstances();
  }

  for (RenderWidgetHostImpl* widget : created_widgets_)
    widget->DetachDelegate();
  created_widgets_.clear();

  if (dialog_manager_)
    dialog_manager_->ResetDialogState(this);

  if (color_chooser_info_.get())
    color_chooser_info_->chooser->End();

  NotifyDisconnected();

  NotificationService::current()->Notify(
      NOTIFICATION_WEB_CONTENTS_DESTROYED,
      Source<WebContents>(this),
      NotificationService::NoDetails());

  frame_tree_.root()->ResetForNewProcess();
  GetRenderManager()->ResetProxyHosts();

  RenderFrameHostManager* root = GetRenderManager();

  if (root->pending_frame_host()) {
    root->pending_frame_host()->SetRenderFrameCreated(false);
    root->pending_frame_host()->SetNavigationHandle(
        scoped_ptr<NavigationHandleImpl>());
  }
  root->current_frame_host()->SetRenderFrameCreated(false);
  root->current_frame_host()->SetNavigationHandle(
      scoped_ptr<NavigationHandleImpl>());

  if (IsBrowserSideNavigationEnabled()) {
    frame_tree_.root()->ResetNavigationRequest(true);
    if (root->speculative_frame_host()) {
      root->speculative_frame_host()->SetRenderFrameCreated(false);
      root->speculative_frame_host()->SetNavigationHandle(
          scoped_ptr<NavigationHandleImpl>());
    }
  }

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    FrameDeleted(root->current_frame_host()));

  if (root->pending_render_view_host()) {
    FOR_EACH_OBSERVER(WebContentsObserver,
                      observers_,
                      RenderViewDeleted(root->pending_render_view_host()));
  }

  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    RenderViewDeleted(root->current_host()));

  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    WebContentsDestroyed());

  FOR_EACH_OBSERVER(WebContentsObserver,
                    observers_,
                    ResetWebContents());

  SetDelegate(NULL);

  STLDeleteContainerPairSecondPointers(destruction_observers_.begin(),
                                       destruction_observers_.end());
}
