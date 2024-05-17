void ViewPainterTest::RunFixedBackgroundTest(
    bool prefer_compositing_to_lcd_text) {
  if (RuntimeEnabledFeatures::SlimmingPaintV2Enabled())
    return;

  if (prefer_compositing_to_lcd_text) {
    Settings* settings = GetDocument().GetFrame()->GetSettings();
    settings->SetPreferCompositingToLCDTextEnabled(true);
  }
  SetBodyInnerHTML(R"HTML(
    <style>
      ::-webkit-scrollbar { display: none; }
      body {
        margin: 0;
        width: 1200px;
        height: 900px;
        background: radial-gradient(
          circle at 100px 100px, blue, transparent 200px) fixed;
      }
    </style>
  )HTML");

  LocalFrameView* frame_view = GetDocument().View();
  ScrollableArea* layout_viewport = frame_view->LayoutViewportScrollableArea();

  ScrollOffset scroll_offset(200, 150);
  layout_viewport->SetScrollOffset(scroll_offset, kUserScroll);
  frame_view->UpdateAllLifecyclePhases();

  CompositedLayerMapping* clm =
      GetLayoutView().Layer()->GetCompositedLayerMapping();

  GraphicsLayer* layer_for_background;
  if (prefer_compositing_to_lcd_text) {
    layer_for_background = clm->MainGraphicsLayer();
  } else {
    layer_for_background = clm->ScrollingContentsLayer();
  }
  const DisplayItemList& display_items =
      layer_for_background->GetPaintController().GetDisplayItemList();
  const DisplayItem& background = display_items[0];
  EXPECT_EQ(background.GetType(), kDocumentBackgroundType);
  DisplayItemClient* expected_client;
  if (!prefer_compositing_to_lcd_text)
    expected_client = GetLayoutView().Layer()->GraphicsLayerBacking();
  else
    expected_client = &GetLayoutView();
  EXPECT_EQ(&background.Client(), expected_client);

  sk_sp<const PaintRecord> record =
      static_cast<const DrawingDisplayItem&>(background).GetPaintRecord();
  ASSERT_EQ(record->size(), 2u);
  cc::PaintOpBuffer::Iterator it(record.get());
  ASSERT_EQ((*++it)->GetType(), cc::PaintOpType::DrawRect);

  SkRect rect = static_cast<const cc::DrawRectOp*>(*it)->rect;
  ASSERT_EQ(prefer_compositing_to_lcd_text ? ScrollOffset() : scroll_offset,
            ScrollOffset(rect.fLeft, rect.fTop));
}
