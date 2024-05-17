size_t IndexedDBDatabase::GetUsableMessageSizeInBytes() const {
  return blink::mojom::kIDBMaxMessageSize -
         blink::mojom::kIDBMaxMessageOverhead;
}
