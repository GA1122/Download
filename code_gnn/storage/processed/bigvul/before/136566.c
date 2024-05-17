String PropertyTreeState::ToString() const {
  return String::Format("t:%p c:%p e:%p", Transform(), Clip(), Effect());
}
