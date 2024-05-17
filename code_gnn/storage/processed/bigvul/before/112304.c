  virtual bool Visit(content::RenderView* render_view) {
    if (render_view->GetRoutingID() == route_id_) {
      view_ = render_view;
      return false;
    }
    return true;
  }
