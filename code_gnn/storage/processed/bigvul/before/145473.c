void AttachRequestBodyBlobDataHandles(
    ResourceRequestBody* body,
    storage::BlobStorageContext* blob_context) {
  DCHECK(blob_context);
  for (size_t i = 0; i < body->elements()->size(); ++i) {
    const ResourceRequestBody::Element& element = (*body->elements())[i];
    if (element.type() != ResourceRequestBody::Element::TYPE_BLOB)
      continue;
    scoped_ptr<storage::BlobDataHandle> handle =
        blob_context->GetBlobDataFromUUID(element.blob_uuid());
    DCHECK(handle);
    if (!handle)
      continue;
    const void* key = handle.get();
    body->SetUserData(key, handle.release());
  }
}
