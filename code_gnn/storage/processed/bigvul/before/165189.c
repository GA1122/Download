std::unique_ptr<DragImage> DragController::DragImageForSelection(
    const LocalFrame& frame,
    float opacity) {
  if (!frame.Selection().ComputeVisibleSelectionInDOMTreeDeprecated().IsRange())
    return nullptr;

  frame.View()->UpdateAllLifecyclePhasesExceptPaint();
  DCHECK(frame.GetDocument()->IsActive());

  FloatRect painting_rect = ClippedSelection(frame);
  GlobalPaintFlags paint_flags =
      kGlobalPaintSelectionOnly | kGlobalPaintFlattenCompositingLayers;

  PaintRecordBuilder builder;
  frame.View()->PaintContents(builder.Context(), paint_flags,
                              EnclosingIntRect(painting_rect));
  return DataTransfer::CreateDragImageForFrame(
      frame, opacity, kDoNotRespectImageOrientation, painting_rect.Size(),
      painting_rect.Location(), builder, PropertyTreeState::Root());
}
