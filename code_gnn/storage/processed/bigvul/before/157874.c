void WebContentsImpl::ShowCreatedWindow(int process_id,
                                        int main_frame_widget_route_id,
                                        WindowOpenDisposition disposition,
                                        const gfx::Rect& initial_rect,
                                        bool user_gesture) {
  std::unique_ptr<WebContents> popup =
      GetCreatedWindow(process_id, main_frame_widget_route_id);
  if (popup) {
    WebContentsImpl* raw_popup = static_cast<WebContentsImpl*>(popup.get());
    WebContentsDelegate* delegate = GetDelegate();
    raw_popup->is_resume_pending_ = true;
    if (!delegate || delegate->ShouldResumeRequestsForCreatedWindow())
      raw_popup->ResumeLoadingCreatedWebContents();

    base::WeakPtr<WebContentsImpl> weak_popup =
        raw_popup->weak_factory_.GetWeakPtr();
    if (delegate) {
      delegate->AddNewContents(this, std::move(popup), disposition,
                               initial_rect, user_gesture, nullptr);
      if (!weak_popup)
        return;   
    }

    RenderWidgetHostImpl* rwh =
        weak_popup->GetMainFrame()->GetRenderWidgetHost();
    DCHECK_EQ(main_frame_widget_route_id, rwh->GetRoutingID());
    rwh->Send(new ViewMsg_SetBounds_ACK(rwh->GetRoutingID()));
  }
}
