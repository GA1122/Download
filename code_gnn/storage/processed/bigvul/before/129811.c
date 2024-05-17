void SystemClipboard::WriteDataObject(DataObject* data_object) {

  HashMap<String, String> custom_data;
  WebDragData data = data_object->ToWebDragData();
  for (const WebDragData::Item& item : data.Items()) {
    if (item.storage_type == WebDragData::Item::kStorageTypeString) {
      if (item.string_type == blink::kMimeTypeTextPlain) {
        clipboard_->WriteText(mojom::ClipboardBuffer::kStandard,
                              NonNullString(item.string_data));
      } else if (item.string_type == blink::kMimeTypeTextHTML) {
        clipboard_->WriteHtml(mojom::ClipboardBuffer::kStandard,
                              NonNullString(item.string_data), KURL());
      } else if (item.string_type != blink::kMimeTypeDownloadURL) {
        custom_data.insert(item.string_type, NonNullString(item.string_data));
      }
    }
  }
  if (!custom_data.IsEmpty()) {
    clipboard_->WriteCustomData(mojom::ClipboardBuffer::kStandard,
                                std::move(custom_data));
  }
  clipboard_->CommitWrite(mojom::ClipboardBuffer::kStandard);
}
