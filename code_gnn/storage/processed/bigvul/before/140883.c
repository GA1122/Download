void PresentationConnectionProxy::OnMessage(
    PresentationConnectionMessage message,
    const OnMessageCallback& callback) {
  DCHECK(!callback.is_null());

  if (message.is_binary()) {
    source_connection_->didReceiveBinaryMessage(&(message.data->front()),
                                                message.data->size());
  } else {
    source_connection_->didReceiveTextMessage(
        blink::WebString::fromUTF8(*(message.message)));
  }

  callback.Run(true);
}
