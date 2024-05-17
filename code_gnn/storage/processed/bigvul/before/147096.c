  virtual float SpoolPage(GraphicsContext& context,
                          int page_number,
                          const IntRect& bounds) {
    IntRect page_rect = page_rects_[page_number];
    float scale = printed_page_width_ / page_rect.Width();

    AffineTransform transform;
#if defined(OS_POSIX) && !defined(OS_MACOSX)
    transform.Scale(scale);
#endif
    transform.Translate(static_cast<float>(-page_rect.X()),
                        static_cast<float>(-page_rect.Y()));
    context.Save();
    context.ConcatCTM(transform);
    context.ClipRect(page_rect);

    PaintRecordBuilder builder(bounds, &context.Canvas()->getMetaData(),
                               &context);

    {
      DisplayItemCacheSkipper skipper(builder.Context());
      GetFrame()->View()->PaintContents(builder.Context(),
                                        kGlobalPaintNormalPhase, page_rect);

      DrawingRecorder line_boundary_recorder(
          builder.Context(), builder,
          DisplayItem::kPrintedContentDestinationLocations, page_rect);
      OutputLinkedDestinations(builder.Context(), page_rect);
    }
    context.DrawRecord(builder.EndRecording());

    context.Restore();

    return scale;
  }
