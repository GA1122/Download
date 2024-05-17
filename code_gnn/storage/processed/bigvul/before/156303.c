void OnEncodingInfoError(
    std::unique_ptr<WebMediaCapabilitiesQueryCallbacks> callbacks) {
  callbacks->OnError();
}
