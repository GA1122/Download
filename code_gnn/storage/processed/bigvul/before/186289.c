  void RenderFrameHostManager::EnsureRenderFrameHostVisibilityConsistent() {
  if (render_frame_host_->GetView() &&
      render_frame_host_->render_view_host()->GetWidget()->is_hidden() !=
          delegate_->IsHidden()) {
//   RenderWidgetHostView* view = GetRenderWidgetHostView();
//   if (view && static_cast<RenderWidgetHostImpl*>(view->GetRenderWidgetHost())
//                       ->is_hidden() != delegate_->IsHidden()) {
      if (delegate_->IsHidden()) {
      render_frame_host_->GetView()->Hide();
//       view->Hide();
      } else {
      render_frame_host_->GetView()->Show();
//       view->Show();
      }
    }
  }