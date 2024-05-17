BOOL CALLBACK SetCutoutRectsCallback(HWND window, LPARAM param) {
  CutoutRectsParams* params = reinterpret_cast<CutoutRectsParams*>(param);

  if (GetProp(window, kWidgetOwnerProperty) == params->widget) {
    HWND parent = params->widget->GetNativeView()->GetDispatcher()->
        host()->GetAcceleratedWidget();
    POINT offset;
    offset.x = offset.y = 0;
    MapWindowPoints(window, parent, &offset, 1);

    std::map<HWND, WebPluginGeometry>::iterator i = params->geometry->begin();
    while (i != params->geometry->end() &&
           i->second.window != window &&
           GetParent(i->second.window) != window) {
      ++i;
    }

    if (i == params->geometry->end()) {
      NOTREACHED();
      return TRUE;
    }

    HRGN hrgn = CreateRectRgn(i->second.clip_rect.x(),
                              i->second.clip_rect.y(),
                              i->second.clip_rect.right(),
                              i->second.clip_rect.bottom());
    std::vector<gfx::Rect> cutout_rects = i->second.cutout_rects;
    for (size_t i = 0; i < params->cutout_rects.size(); ++i) {
      gfx::Rect offset_cutout = params->cutout_rects[i];
      offset_cutout.Offset(-offset.x, -offset.y);
      cutout_rects.push_back(offset_cutout);
    }
    gfx::SubtractRectanglesFromRegion(hrgn, cutout_rects);
    SetWindowRgn(window, hrgn, TRUE);
  }
  return TRUE;
}
