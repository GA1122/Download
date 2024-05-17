void DevToolsWindow::AddNewContents(WebContents* source,
                                    WebContents* new_contents,
                                    WindowOpenDisposition disposition,
                                    const gfx::Rect& initial_rect,
                                    bool user_gesture,
                                    bool* was_blocked) {
  if (new_contents == toolbox_web_contents_) {
    toolbox_web_contents_->SetDelegate(
        new DevToolsToolboxDelegate(toolbox_web_contents_,
                                    inspected_contents_observer_.get()));
    if (main_web_contents_->GetRenderWidgetHostView() &&
        toolbox_web_contents_->GetRenderWidgetHostView()) {
      gfx::Size size =
          main_web_contents_->GetRenderWidgetHostView()->GetViewBounds().size();
      toolbox_web_contents_->GetRenderWidgetHostView()->SetSize(size);
    }
    UpdateBrowserWindow();
    return;
  }

  WebContents* inspected_web_contents = GetInspectedWebContents();
  if (inspected_web_contents) {
    inspected_web_contents->GetDelegate()->AddNewContents(
        source, new_contents, disposition, initial_rect, user_gesture,
        was_blocked);
  }
}
