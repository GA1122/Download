void WebContentsImpl::ShowCreatedWindow(int process_id,
                                        int main_frame_widget_route_id,
                                        WindowOpenDisposition disposition,
                                        const gfx::Rect& initial_rect,
                                        bool user_gesture) {
  WebContentsImpl* popup =
      GetCreatedWindow(process_id, main_frame_widget_route_id);
  if (popup) {
    WebContentsDelegate* delegate = GetDelegate();
    popup->is_resume_pending_ = true;
    if (!delegate || delegate->ShouldResumeRequestsForCreatedWindow())
      popup->ResumeLoadingCreatedWebContents();

    if (delegate) {
      base::WeakPtr<WebContentsImpl> weak_popup =
          popup->weak_factory_.GetWeakPtr();
      delegate->AddNewContents(this, popup, disposition, initial_rect,
                               user_gesture, nullptr);
      if (!weak_popup)
        return;   
    }

    RenderWidgetHostImpl* rwh = popup->GetMainFrame()->GetRenderWidgetHost();
    DCHECK_EQ(main_frame_widget_route_id, rwh->GetRoutingID());
    rwh->Send(new ViewMsg_Move_ACK(rwh->GetRoutingID()));
  }
}
