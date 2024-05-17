  void SendFakeScreenOrientation(unsigned angle, const std::string& strType) {
    RenderWidgetHost* rwh = shell()->web_contents()->GetRenderWidgetHostView()
        ->GetRenderWidgetHost();
    blink::WebScreenInfo screen_info;
    rwh->GetWebScreenInfo(&screen_info);
    screen_info.orientationAngle = angle;

    blink::WebScreenOrientationType type = blink::WebScreenOrientationUndefined;
    if (strType == "portrait-primary") {
      type = blink::WebScreenOrientationPortraitPrimary;
    } else if (strType == "portrait-secondary") {
      type = blink::WebScreenOrientationPortraitSecondary;
    } else if (strType == "landscape-primary") {
      type = blink::WebScreenOrientationLandscapePrimary;
    } else if (strType == "landscape-secondary") {
      type = blink::WebScreenOrientationLandscapeSecondary;
    }
    ASSERT_NE(blink::WebScreenOrientationUndefined, type);
    screen_info.orientationType = type;

    ViewMsg_Resize_Params params;
    params.screen_info = screen_info;
    params.new_size = gfx::Size(0, 0);
    params.physical_backing_size = gfx::Size(300, 300);
    params.top_controls_layout_height = 0.f;
    params.resizer_rect = gfx::Rect();
    params.is_fullscreen = false;
    rwh->Send(new ViewMsg_Resize(rwh->GetRoutingID(), params));
  }
