Response InspectorPageAgent::getLayoutMetrics(
    std::unique_ptr<protocol::Page::LayoutViewport>* out_layout_viewport,
    std::unique_ptr<protocol::Page::VisualViewport>* out_visual_viewport,
    std::unique_ptr<protocol::DOM::Rect>* out_content_size) {
  LocalFrame* main_frame = inspected_frames_->Root();
  VisualViewport& visual_viewport = main_frame->GetPage()->GetVisualViewport();

  main_frame->GetDocument()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  IntRect visible_contents = main_frame->View()->VisibleContentRect();
  *out_layout_viewport = protocol::Page::LayoutViewport::create()
                             .setPageX(visible_contents.X())
                             .setPageY(visible_contents.Y())
                             .setClientWidth(visible_contents.Width())
                             .setClientHeight(visible_contents.Height())
                             .build();

  LocalFrameView* frame_view = main_frame->View();
  ScrollOffset page_offset = frame_view->GetScrollableArea()->GetScrollOffset();
  float page_zoom = main_frame->PageZoomFactor();
  FloatRect visible_rect = visual_viewport.VisibleRect();
  float scale = visual_viewport.Scale();
  float scrollbar_width = frame_view->VerticalScrollbarWidth() / scale;
  float scrollbar_height = frame_view->HorizontalScrollbarHeight() / scale;

  IntSize content_size = frame_view->GetScrollableArea()->ContentsSize();
  *out_content_size = protocol::DOM::Rect::create()
                          .setX(0)
                          .setY(0)
                          .setWidth(content_size.Width())
                          .setHeight(content_size.Height())
                          .build();

  *out_visual_viewport =
      protocol::Page::VisualViewport::create()
          .setOffsetX(AdjustScrollForAbsoluteZoom(visible_rect.X(), page_zoom))
          .setOffsetY(AdjustScrollForAbsoluteZoom(visible_rect.Y(), page_zoom))
          .setPageX(AdjustScrollForAbsoluteZoom(page_offset.Width(), page_zoom))
          .setPageY(
              AdjustScrollForAbsoluteZoom(page_offset.Height(), page_zoom))
          .setClientWidth(visible_rect.Width() - scrollbar_width)
          .setClientHeight(visible_rect.Height() - scrollbar_height)
          .setScale(scale)
          .build();
  return Response::OK();
}
