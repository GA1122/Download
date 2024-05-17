void Document::InheritHtmlAndBodyElementStyles(StyleRecalcChange change) {
  DCHECK(InStyleRecalc());
  DCHECK(documentElement());

  bool did_recalc_document_element = false;
  RefPtr<ComputedStyle> document_element_style =
      documentElement()->MutableComputedStyle();
  if (change == kForce)
    documentElement()->ClearAnimationStyleChange();
  if (!document_element_style || documentElement()->NeedsStyleRecalc() ||
      change == kForce) {
    document_element_style =
        EnsureStyleResolver().StyleForElement(documentElement());
    did_recalc_document_element = true;
  }

  WritingMode root_writing_mode = document_element_style->GetWritingMode();
  TextDirection root_direction = document_element_style->Direction();

  HTMLElement* body = this->body();
  RefPtr<ComputedStyle> body_style;

  if (body) {
    body_style = body->MutableComputedStyle();
    if (did_recalc_document_element)
      body->ClearAnimationStyleChange();
    if (!body_style || body->NeedsStyleRecalc() ||
        did_recalc_document_element) {
      body_style = EnsureStyleResolver().StyleForElement(
          body, document_element_style.Get(), document_element_style.Get());
    }
    root_writing_mode = body_style->GetWritingMode();
    root_direction = body_style->Direction();
  }

  const ComputedStyle* background_style = document_element_style.Get();
  if (isHTMLHtmlElement(documentElement()) && isHTMLBodyElement(body) &&
      !background_style->HasBackground())
    background_style = body_style.Get();

  Node& root_scroller = GetRootScrollerController().EffectiveRootScroller();
  RefPtr<ComputedStyle> root_scroller_style;
  if (this != &root_scroller) {
    DCHECK(root_scroller.IsElementNode());
    Element* root_scroller_element = ToElement(&root_scroller);
    root_scroller_style = root_scroller_element->MutableComputedStyle();

    if (!root_scroller_style || root_scroller_element->NeedsStyleRecalc()) {
      root_scroller_style =
          EnsureStyleResolver().StyleForElement(root_scroller_element);
    }

    background_style = root_scroller_style.Get();
  }

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
  if (Element* element =
          ViewportDefiningElement(document_element_style.Get())) {
    if (element == body) {
      overflow_style = body_style.Get();
    } else {
      DCHECK_EQ(element, documentElement());
      overflow_style = document_element_style.Get();

      if (body_style && !body_style->IsOverflowVisible())
        UseCounter::Count(*this, WebFeature::kBodyScrollsInAdditionToViewport);
    }
  }

  if (GetStyleEngine().UsesRemUnits() &&
      (documentElement()->NeedsAttach() ||
       !documentElement()->GetComputedStyle() ||
       documentElement()->GetComputedStyle()->FontSize() !=
           document_element_style->FontSize())) {
    EnsureStyleResolver().InvalidateMatchedPropertiesCache();
    documentElement()->SetNeedsStyleRecalc(
        kSubtreeStyleChange, StyleChangeReasonForTracing::Create(
                                 StyleChangeReason::kFontSizeChange));
  }

  EOverflowAnchor overflow_anchor = EOverflowAnchor::kAuto;
  EOverflow overflow_x = EOverflow::kAuto;
  EOverflow overflow_y = EOverflow::kAuto;
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
    column_gap = overflow_style->ColumnGap();
  }

  ScrollSnapType snap_type = overflow_style->GetScrollSnapType();

  RefPtr<ComputedStyle> viewport_style = GetLayoutViewItem().MutableStyle();
  if (viewport_style->GetWritingMode() != root_writing_mode ||
      viewport_style->Direction() != root_direction ||
      viewport_style->VisitedDependentColor(CSSPropertyBackgroundColor) !=
          background_color ||
      viewport_style->BackgroundLayers() != background_layers ||
      viewport_style->ImageRendering() != image_rendering ||
      viewport_style->OverflowAnchor() != overflow_anchor ||
      viewport_style->OverflowX() != overflow_x ||
      viewport_style->OverflowY() != overflow_y ||
      viewport_style->ColumnGap() != column_gap ||
      viewport_style->GetScrollSnapType() != snap_type) {
    RefPtr<ComputedStyle> new_style = ComputedStyle::Clone(*viewport_style);
    new_style->SetWritingMode(root_writing_mode);
    new_style->SetDirection(root_direction);
    new_style->SetBackgroundColor(background_color);
    new_style->AccessBackgroundLayers() = background_layers;
    new_style->SetImageRendering(image_rendering);
    new_style->SetOverflowAnchor(overflow_anchor);
    new_style->SetOverflowX(overflow_x);
    new_style->SetOverflowY(overflow_y);
    new_style->SetColumnGap(column_gap);
    new_style->SetScrollSnapType(snap_type);
    GetLayoutViewItem().SetStyle(new_style);
    SetupFontBuilder(*new_style);
  }

  if (body) {
    if (const ComputedStyle* style = body->GetComputedStyle()) {
      if (style->Direction() != root_direction ||
          style->GetWritingMode() != root_writing_mode)
        body->SetNeedsStyleRecalc(kSubtreeStyleChange,
                                  StyleChangeReasonForTracing::Create(
                                      StyleChangeReason::kWritingModeChange));
    }
  }

  if (const ComputedStyle* style = documentElement()->GetComputedStyle()) {
    if (style->Direction() != root_direction ||
        style->GetWritingMode() != root_writing_mode)
      documentElement()->SetNeedsStyleRecalc(
          kSubtreeStyleChange, StyleChangeReasonForTracing::Create(
                                   StyleChangeReason::kWritingModeChange));
  }
}
