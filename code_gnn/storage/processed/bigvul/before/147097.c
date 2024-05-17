  float SpoolSinglePage(WebCanvas* canvas, int page_number) {
    DispatchEventsForPrintingOnAllFrames();
    if (!GetFrame()->GetDocument() ||
        GetFrame()->GetDocument()->GetLayoutViewItem().IsNull())
      return 0;

    GetFrame()->View()->UpdateAllLifecyclePhasesExceptPaint();
    if (!GetFrame()->GetDocument() ||
        GetFrame()->GetDocument()->GetLayoutViewItem().IsNull())
      return 0;

    IntRect bounds(0, 0, printed_page_height_, printed_page_width_);
    PaintRecordBuilder builder(bounds, &canvas->getMetaData());
    builder.Context().SetPrinting(true);
    builder.Context().BeginRecording(bounds);
    float scale = SpoolPage(builder.Context(), page_number, bounds);
    canvas->drawPicture(builder.Context().EndRecording());
    return scale;
  }
