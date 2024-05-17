LRESULT CALLBACK TreeView::TreeWndProc(HWND window,
                                       UINT message,
                                       WPARAM w_param,
                                       LPARAM l_param) {
  TreeViewWrapper* wrapper = reinterpret_cast<TreeViewWrapper*>(
      GetWindowLongPtr(window, GWLP_USERDATA));
  DCHECK(wrapper);
  TreeView* tree = wrapper->tree_view;

  switch (message) {
    case WM_ERASEBKGND:
      return 1;

    case WM_PAINT: {
      gfx::CanvasSkiaPaint canvas(window);
      if (canvas.isEmpty())
        return 0;

      HDC dc = skia::BeginPlatformPaint(&canvas);
      if (base::i18n::IsRTL()) {
        XFORM xform = {0};
        xform.eM11 = xform.eM22 = 1;
        SetWorldTransform(dc, &xform);

        SetGraphicsMode(dc, GM_COMPATIBLE);
        SetLayout(dc, LAYOUT_RTL);

        SetViewportOrgEx(dc, -canvas.paintStruct().rcPaint.left,
                         -canvas.paintStruct().rcPaint.top, NULL);
      }
      SendMessage(window, WM_PRINTCLIENT, reinterpret_cast<WPARAM>(dc), 0);
      if (base::i18n::IsRTL()) {
        SetViewportOrgEx(dc, 0, 0, NULL);
      }
      skia::EndPlatformPaint(&canvas);
      return 0;
    }

    case WM_RBUTTONDOWN:
      if (tree->select_on_right_mouse_down_) {
        TVHITTESTINFO hit_info;
        hit_info.pt = gfx::Point(l_param).ToPOINT();
        HTREEITEM hit_item = TreeView_HitTest(window, &hit_info);
        if (hit_item && (hit_info.flags & (TVHT_ONITEM | TVHT_ONITEMRIGHT |
                                           TVHT_ONITEMINDENT)) != 0)
          TreeView_SelectItem(tree->tree_view_, hit_item);
      }
      break;
  }
  WNDPROC handler = tree->original_handler_;
  DCHECK(handler);
  return CallWindowProc(handler, window, message, w_param, l_param);
}
