bool ValidateSubSourceAndGetData(DOMArrayBufferView* view,
                                 GLuint sub_offset,
                                 GLuint sub_length,
                                 void** out_base_address,
                                 long long* out_byte_length) {
  DCHECK(view);

  size_t type_size = view->TypeSize();
  DCHECK_GE(8u, type_size);
  long long byte_length = 0;
  if (sub_length) {
    byte_length = sub_length * type_size;
  }
  long long byte_offset = 0;
  if (sub_offset) {
    byte_offset = sub_offset * type_size;
  }
  CheckedNumeric<long long> total = byte_offset;
  total += byte_length;
  if (!total.IsValid() || total.ValueOrDie() > view->byteLength()) {
    return false;
  }
  if (!byte_length) {
    byte_length = view->byteLength() - byte_offset;
  }
  uint8_t* data = static_cast<uint8_t*>(view->BaseAddressMaybeShared());
  data += byte_offset;
  *out_base_address = data;
  *out_byte_length = byte_length;
  return true;
}
