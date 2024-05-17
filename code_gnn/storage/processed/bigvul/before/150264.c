void InputEngine::GetRulebasedKeypressCountForTesting(
    GetRulebasedKeypressCountForTestingCallback callback) {
  auto& context = channel_receivers_.current_context();
  auto& engine = context.get()->engine;
  std::move(callback).Run(engine ? engine->process_key_count() : -1);
}
