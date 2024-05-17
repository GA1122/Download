 void Editor::AddToKillRing(const EphemeralRange& range) {
  if (should_start_new_kill_ring_sequence_)
    GetKillRing().StartNewSequence();

  DCHECK(!GetFrame().GetDocument()->NeedsLayoutTreeUpdate());
  String text = PlainText(range);
  GetKillRing().Append(text);
  should_start_new_kill_ring_sequence_ = false;
}
