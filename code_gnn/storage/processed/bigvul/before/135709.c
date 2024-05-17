void GranularityStrategyTest::SetUp() {
  dummy_page_holder_ = DummyPageHolder::Create(IntSize(800, 600));
  document_ = &dummy_page_holder_->GetDocument();
  DCHECK(document_);
  GetDummyPageHolder().GetFrame().GetSettings()->SetDefaultFontSize(12);
  GetDummyPageHolder().GetFrame().GetSettings()->SetSelectionStrategy(
      SelectionStrategy::kDirection);
}
