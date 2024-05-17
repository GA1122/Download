  void SendScreenRects() {
    RenderWidgetHostImpl::From(view_->web_contents_->GetRenderViewHost())->
        SendScreenRects();
  }
