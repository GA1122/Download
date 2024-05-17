void RenderWidget::Init(int32 opener_id) {
  DCHECK(!webwidget_);

  if (opener_id != MSG_ROUTING_NONE)
    opener_id_ = opener_id;

  webwidget_ = WebPopupMenu::create(this);

  bool result = render_thread_->Send(
      new ViewHostMsg_CreateWidget(opener_id, popup_type_, &routing_id_));
  if (result) {
    render_thread_->AddRoute(routing_id_, this);
    AddRef();
  } else {
    DCHECK(false);
  }
}
