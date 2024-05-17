GranularityStrategy* FrameSelection::GetGranularityStrategy() {
  SelectionStrategy strategy_type = SelectionStrategy::kCharacter;
  Settings* settings = frame_ ? frame_->GetSettings() : 0;
  if (settings &&
      settings->GetSelectionStrategy() == SelectionStrategy::kDirection)
    strategy_type = SelectionStrategy::kDirection;

  if (granularity_strategy_ &&
      granularity_strategy_->GetType() == strategy_type)
    return granularity_strategy_.get();

  if (strategy_type == SelectionStrategy::kDirection)
    granularity_strategy_ = WTF::MakeUnique<DirectionGranularityStrategy>();
  else
    granularity_strategy_ = WTF::MakeUnique<CharacterGranularityStrategy>();
  return granularity_strategy_.get();
}
