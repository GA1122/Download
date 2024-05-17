  virtual float SpoolPage(GraphicsContext& context, int page_number) {
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

    auto* frame_view = GetFrame()->View();
    DCHECK(frame_view);
    PropertyTreeState property_tree_state =
        frame_view->GetLayoutView()->FirstFragment().LocalBorderBoxProperties();

    PaintRecordBuilder builder(&context.Canvas()->getMetaData(), &context);

    frame_view->PaintContents(builder.Context(), kGlobalPaintNormalPhase,
                              page_rect);
    {
      ScopedPaintChunkProperties scoped_paint_chunk_properties(
          builder.Context().GetPaintController(), property_tree_state, builder,
          DisplayItem::kPrintedContentDestinationLocations);
      DrawingRecorder line_boundary_recorder(
          builder.Context(), builder,
          DisplayItem::kPrintedContentDestinationLocations);
      OutputLinkedDestinations(builder.Context(), page_rect);
    }

    context.DrawRecord(builder.EndRecording(property_tree_state));
    context.Restore();

    return scale;
  }
