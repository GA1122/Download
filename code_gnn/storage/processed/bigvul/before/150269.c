void InputEngine::ResetForRulebased() {
  auto& context = channel_receivers_.current_context();
  auto& engine = context.get()->engine;
  if (engine) {
    engine->Reset();
  }
}
