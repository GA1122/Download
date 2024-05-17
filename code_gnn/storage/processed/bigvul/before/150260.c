bool InputEngine::BindRequest(
    const std::string& ime_spec,
    mojo::PendingReceiver<mojom::InputChannel> receiver,
    mojo::PendingRemote<mojom::InputChannel> remote,
    const std::vector<uint8_t>& extra) {
  if (!IsImeSupportedByRulebased(ime_spec))
    return false;

  channel_receivers_.Add(this, std::move(receiver),
                         std::make_unique<InputEngineContext>(ime_spec));

  return true;
}
