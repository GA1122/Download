const char* WebGL2RenderingContextBase::ValidateGetBufferSubDataBounds(
    const char* function_name,
    WebGLBuffer* source_buffer,
    GLintptr source_byte_offset,
    long long destination_byte_length) {
  CheckedNumeric<long long> src_end = source_byte_offset;
  src_end += destination_byte_length;
  if (!src_end.IsValid() || src_end.ValueOrDie() > source_buffer->GetSize()) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name,
                      "overflow of bound buffer");
    return "Invalid value: overflow of bound buffer";
  }

  return nullptr;
}
