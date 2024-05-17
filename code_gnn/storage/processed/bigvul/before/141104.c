TextAutosizer* Document::GetTextAutosizer() {
  if (!text_autosizer_)
    text_autosizer_ = TextAutosizer::Create(this);
  return text_autosizer_.Get();
}
