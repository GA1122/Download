  RenderViewHost* render_view_host() {
    return browser()->tab_strip_model()->GetActiveWebContents()->
        GetRenderViewHost();
  }
