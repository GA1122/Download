void PaintLayerScrollableArea::Resize(const IntPoint& pos,
                                      const LayoutSize& old_offset) {
  if (!InResizeMode() || !GetLayoutBox()->CanResize() ||
      !GetLayoutBox()->GetNode())
    return;

  DCHECK(GetLayoutBox()->GetNode()->IsElementNode());
  Element* element = ToElement(GetLayoutBox()->GetNode());

  Document& document = element->GetDocument();

  float zoom_factor = GetLayoutBox()->StyleRef().EffectiveZoom();

  IntSize new_offset =
      OffsetFromResizeCorner(document.View()->ConvertFromRootFrame(pos));
  new_offset.SetWidth(new_offset.Width() / zoom_factor);
  new_offset.SetHeight(new_offset.Height() / zoom_factor);

  LayoutSize current_size = GetLayoutBox()->Size();
  current_size.Scale(1 / zoom_factor);

  LayoutSize adjusted_old_offset = LayoutSize(
      old_offset.Width() / zoom_factor, old_offset.Height() / zoom_factor);
  if (GetLayoutBox()->ShouldPlaceBlockDirectionScrollbarOnLogicalLeft()) {
    new_offset.SetWidth(-new_offset.Width());
    adjusted_old_offset.SetWidth(-adjusted_old_offset.Width());
  }

  LayoutSize difference((current_size + new_offset - adjusted_old_offset)
                            .ExpandedTo(MinimumSizeForResizing(zoom_factor)) -
                        current_size);

  bool is_box_sizing_border =
      GetLayoutBox()->StyleRef().BoxSizing() == EBoxSizing::kBorderBox;

  EResize resize =
      ResolvedResize(GetLayoutBox()->StyleRef(),
                     GetLayoutBox()->ContainingBlock()->StyleRef());
  if (resize != EResize::kVertical && difference.Width()) {
    if (element->IsFormControlElement()) {
      element->SetInlineStyleProperty(
          CSSPropertyMarginLeft, GetLayoutBox()->MarginLeft() / zoom_factor,
          CSSPrimitiveValue::UnitType::kPixels);
      element->SetInlineStyleProperty(
          CSSPropertyMarginRight, GetLayoutBox()->MarginRight() / zoom_factor,
          CSSPrimitiveValue::UnitType::kPixels);
    }
    LayoutUnit base_width =
        GetLayoutBox()->Size().Width() -
        (is_box_sizing_border ? LayoutUnit()
                              : GetLayoutBox()->BorderAndPaddingWidth());
    base_width = LayoutUnit(base_width / zoom_factor);
    element->SetInlineStyleProperty(CSSPropertyWidth,
                                    RoundToInt(base_width + difference.Width()),
                                    CSSPrimitiveValue::UnitType::kPixels);
  }

  if (resize != EResize::kHorizontal && difference.Height()) {
    if (element->IsFormControlElement()) {
      element->SetInlineStyleProperty(CSSPropertyMarginTop,
                                      GetLayoutBox()->MarginTop() / zoom_factor,
                                      CSSPrimitiveValue::UnitType::kPixels);
      element->SetInlineStyleProperty(
          CSSPropertyMarginBottom, GetLayoutBox()->MarginBottom() / zoom_factor,
          CSSPrimitiveValue::UnitType::kPixels);
    }
    LayoutUnit base_height =
        GetLayoutBox()->Size().Height() -
        (is_box_sizing_border ? LayoutUnit()
                              : GetLayoutBox()->BorderAndPaddingHeight());
    base_height = LayoutUnit(base_height / zoom_factor);
    element->SetInlineStyleProperty(
        CSSPropertyHeight, RoundToInt(base_height + difference.Height()),
        CSSPrimitiveValue::UnitType::kPixels);
  }

  document.UpdateStyleAndLayout();

}
