blink::mojom::BlobURLTokenPtrInfo CloneBlobURLToken(
    mojo::MessagePipeHandle handle) {
  if (!handle.is_valid())
    return nullptr;
  blink::mojom::BlobURLTokenPtrInfo result;
  blink::mojom::BlobURLTokenPtr token(
      blink::mojom::BlobURLTokenPtrInfo(mojo::ScopedMessagePipeHandle(handle),
                                        blink::mojom::BlobURLToken::Version_));
  token->Clone(MakeRequest(&result));
  ignore_result(token.PassInterface().PassHandle().release());
  return result;
}
