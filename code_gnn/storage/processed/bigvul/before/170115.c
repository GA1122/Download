void InputConnectionImpl::RequestTextInputState(
    mojom::InputConnection::RequestTextInputStateCallback callback) {
  std::move(callback).Run(GetTextInputState(false));
}
