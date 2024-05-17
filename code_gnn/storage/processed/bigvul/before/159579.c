void Document::PropagateStyleToViewport() {
  DCHECK(InStyleRecalc());
  DCHECK(documentElement());

  HTMLElement* body = this->body();

  const ComputedStyle* body_style =
      body ? body->EnsureComputedStyle() : nullptr;
  const ComputedStyle* document_element_style =
      documentElement()->EnsureComputedStyle();

  WritingMode root_writing_mode = document_element_style->GetWritingMode();
  TextDirection root_direction = document_element_style->Direction();
  if (body_style) {
    root_writing_mode = body_style->GetWritingMode();
    root_direction = body_style->Direction();
  }

  const ComputedStyle* background_style = document_element_style;
  if (IsHTMLHtmlElement(documentElement()) && IsHTMLBodyElement(body) &&
      !background_style->HasBackground())
    background_style = body_style;

  Color background_color =
      background_style->VisitedDependentColor(CSSPropertyBackgroundColor);
  FillLayer background_layers = background_style->BackgroundLayers();
  for (auto current_layer = &background_layers; current_layer;
       current_layer = current_layer->Next()) {
    current_layer->SetClip(kBorderFillBox);

    if (current_layer->Attachment() == kScrollBackgroundAttachment)
      current_layer->SetAttachment(kLocalBackgroundAttachment);
  }
  EImageRendering image_rendering = background_style->ImageRendering();

  const ComputedStyle* overflow_style = nullptr;
  if (Element* element = ViewportDefiningElement(document_element_style)) {
    if (element == body) {
      overflow_style = body_style;
    } else {
      DCHECK_EQ(element, documentElement());
      overflow_style = document_element_style;

      if (body_style && !body_style->IsOverflowVisible())
        UseCounter::Count(*this, WebFeature::kBodyScrollsInAdditionToViewport);
    }
  }

  EOverflowAnchor overflow_anchor = EOverflowAnchor::kAuto;
  EOverflow overflow_x = EOverflow::kAuto;
  EOverflow overflow_y = EOverflow::kAuto;
  bool column_gap_normal = true;
  float column_gap = 0;
  if (overflow_style) {
    overflow_anchor = overflow_style->OverflowAnchor();
    overflow_x = overflow_style->OverflowX();
    overflow_y = overflow_style->OverflowY();
    if (overflow_x == EOverflow::kVisible)
      overflow_x = EOverflow::kAuto;
    if (overflow_y == EOverflow::kVisible)
      overflow_y = EOverflow::kAuto;
    if (overflow_anchor == EOverflowAnchor::kVisible)
      overflow_anchor = EOverflowAnchor::kAuto;
    if (!overflow_style->HasNormalColumnGap()) {
      column_gap_normal = false;
      column_gap = overflow_style->ColumnGap();
    }
  }

  ScrollSnapType snap_type = overflow_style->GetScrollSnapType();
  ScrollBehavior scroll_behavior = document_element_style->GetScrollBehavior();

  EScrollBoundaryBehavior scroll_boundary_behavior_x =
      overflow_style->OverscrollBehaviorX();
  EScrollBoundaryBehavior scroll_boundary_behavior_y =
      overflow_style->OverscrollBehaviorY();
  using ScrollBoundaryBehaviorType =
      WebScrollBoundaryBehavior::ScrollBoundaryBehaviorType;
  if (RuntimeEnabledFeatures::CSSScrollBoundaryBehaviorEnabled() &&
      IsInMainFrame()) {
    GetPage()->GetOverscrollController().SetScrollBoundaryBehavior(
        WebScrollBoundaryBehavior(
            static_cast<ScrollBoundaryBehaviorType>(scroll_boundary_behavior_x),
            static_cast<ScrollBoundaryBehaviorType>(
                scroll_boundary_behavior_y)));
  }

  scoped_refptr<ComputedStyle> viewport_style =
      GetLayoutViewItem().MutableStyle();
  if (viewport_style->GetWritingMode() != root_writing_mode ||
      viewport_style->Direction() != root_direction ||
      viewport_style->VisitedDependentColor(CSSPropertyBackgroundColor) !=
          background_color ||
      viewport_style->BackgroundLayers() != background_layers ||
      viewport_style->ImageRendering() != image_rendering ||
      viewport_style->OverflowAnchor() != overflow_anchor ||
      viewport_style->OverflowX() != overflow_x ||
      viewport_style->OverflowY() != overflow_y ||
      viewport_style->HasNormalColumnGap() != column_gap_normal ||
      viewport_style->ColumnGap() != column_gap ||
      viewport_style->GetScrollSnapType() != snap_type ||
      viewport_style->GetScrollBehavior() != scroll_behavior ||
      viewport_style->OverscrollBehaviorX() != scroll_boundary_behavior_x ||
      viewport_style->OverscrollBehaviorY() != scroll_boundary_behavior_y) {
    scoped_refptr<ComputedStyle> new_style =
        ComputedStyle::Clone(*viewport_style);
    new_style->SetWritingMode(root_writing_mode);
    new_style->SetDirection(root_direction);
    new_style->SetBackgroundColor(background_color);
    new_style->AccessBackgroundLayers() = background_layers;
    new_style->SetImageRendering(image_rendering);
    new_style->SetOverflowAnchor(overflow_anchor);
    new_style->SetOverflowX(overflow_x);
    new_style->SetOverflowY(overflow_y);
    if (column_gap_normal)
      new_style->SetHasNormalColumnGap();
    else
      new_style->SetColumnGap(column_gap);
    new_style->SetScrollSnapType(snap_type);
    new_style->SetScrollBehavior(scroll_behavior);
    new_style->SetOverscrollBehaviorX(scroll_boundary_behavior_x);
    new_style->SetOverscrollBehaviorY(scroll_boundary_behavior_y);
    GetLayoutViewItem().SetStyle(new_style);
    SetupFontBuilder(*new_style);
  }
}
