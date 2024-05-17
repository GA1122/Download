  void SpoolAllPagesWithBoundariesForTesting(
      WebCanvas* canvas,
      const FloatSize& page_size_in_pixels) {
    DispatchEventsForPrintingOnAllFrames();
    if (!GetFrame()->GetDocument() ||
        GetFrame()->GetDocument()->GetLayoutViewItem().IsNull())
      return;

    GetFrame()->View()->UpdateAllLifecyclePhasesExceptPaint();
    if (!GetFrame()->GetDocument() ||
        GetFrame()->GetDocument()->GetLayoutViewItem().IsNull())
      return;

    ComputePageRects(page_size_in_pixels);

    const float page_width = page_size_in_pixels.Width();
    size_t num_pages = PageRects().size();
    int total_height = num_pages * (page_size_in_pixels.Height() + 1) - 1;
    IntRect all_pages_rect(0, 0, page_width, total_height);

    PaintRecordBuilder builder(all_pages_rect, &canvas->getMetaData());
    GraphicsContext& context = builder.Context();
    context.SetPrinting(true);
    context.BeginRecording(all_pages_rect);

    context.FillRect(FloatRect(0, 0, page_width, total_height), Color::kWhite);

    int current_height = 0;
    for (size_t page_index = 0; page_index < num_pages; page_index++) {
      if (page_index > 0) {
        context.Save();
        context.SetStrokeColor(Color(0, 0, 255));
        context.SetFillColor(Color(0, 0, 255));
        context.DrawLine(IntPoint(0, current_height),
                         IntPoint(page_width, current_height));
        context.Restore();
      }

      AffineTransform transform;
      transform.Translate(0, current_height);
#if defined(OS_WIN) || defined(OS_MACOSX)
      float scale = GetPageShrink(page_index);
      transform.Scale(scale, scale);
#endif
      context.Save();
      context.ConcatCTM(transform);

      SpoolPage(context, page_index, all_pages_rect);

      context.Restore();

      current_height += page_size_in_pixels.Height() + 1;
    }
    canvas->drawPicture(context.EndRecording());
  }
