  float SpoolSinglePage(cc::PaintCanvas* canvas, int page_number) {
    DispatchEventsForPrintingOnAllFrames();
    if (!GetFrame()->GetDocument() ||
        !GetFrame()->GetDocument()->GetLayoutView())
      return 0;

    GetFrame()->View()->UpdateLifecyclePhasesForPrinting();
    if (!GetFrame()->GetDocument() ||
        !GetFrame()->GetDocument()->GetLayoutView())
      return 0;

    FloatRect bounds(0, 0, printed_page_height_, printed_page_width_);
    PaintRecordBuilder builder(&canvas->getMetaData());
    builder.Context().SetPrinting(true);
    builder.Context().BeginRecording(bounds);
    float scale = SpoolPage(builder.Context(), page_number);
    canvas->drawPicture(builder.Context().EndRecording());
    return scale;
  }
