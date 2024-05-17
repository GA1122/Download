bool WebGLRenderingContextBase::ValidateTexFuncData(
    const char* function_name,
    TexImageDimension tex_dimension,
    GLint level,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    GLenum format,
    GLenum type,
    DOMArrayBufferView* pixels,
    NullDisposition disposition,
    GLuint src_offset) {
  if (!pixels) {
    DCHECK_NE(disposition, kNullNotReachable);
    if (disposition == kNullAllowed)
      return true;
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "no pixels");
    return false;
  }

  if (!ValidateSettableTexFormat(function_name, format))
    return false;

  switch (type) {
    case GL_BYTE:
      if (pixels->GetType() != DOMArrayBufferView::kTypeInt8) {
        SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                          "type BYTE but ArrayBufferView not Int8Array");
        return false;
      }
      break;
    case GL_UNSIGNED_BYTE:
      if (pixels->GetType() != DOMArrayBufferView::kTypeUint8) {
        SynthesizeGLError(
            GL_INVALID_OPERATION, function_name,
            "type UNSIGNED_BYTE but ArrayBufferView not Uint8Array");
        return false;
      }
      break;
    case GL_SHORT:
      if (pixels->GetType() != DOMArrayBufferView::kTypeInt16) {
        SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                          "type SHORT but ArrayBufferView not Int16Array");
        return false;
      }
      break;
    case GL_UNSIGNED_SHORT:
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_5_5_5_1:
      if (pixels->GetType() != DOMArrayBufferView::kTypeUint16) {
        SynthesizeGLError(
            GL_INVALID_OPERATION, function_name,
            "type UNSIGNED_SHORT but ArrayBufferView not Uint16Array");
        return false;
      }
      break;
    case GL_INT:
      if (pixels->GetType() != DOMArrayBufferView::kTypeInt32) {
        SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                          "type INT but ArrayBufferView not Int32Array");
        return false;
      }
      break;
    case GL_UNSIGNED_INT:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_10F_11F_11F_REV:
    case GL_UNSIGNED_INT_5_9_9_9_REV:
    case GL_UNSIGNED_INT_24_8:
      if (pixels->GetType() != DOMArrayBufferView::kTypeUint32) {
        SynthesizeGLError(
            GL_INVALID_OPERATION, function_name,
            "type UNSIGNED_INT but ArrayBufferView not Uint32Array");
        return false;
      }
      break;
    case GL_FLOAT:   
      if (pixels->GetType() != DOMArrayBufferView::kTypeFloat32) {
        SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                          "type FLOAT but ArrayBufferView not Float32Array");
        return false;
      }
      break;
    case GL_HALF_FLOAT:
    case GL_HALF_FLOAT_OES:   
      if (pixels->GetType() != DOMArrayBufferView::kTypeUint16) {
        SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                          "type HALF_FLOAT_OES but ArrayBufferView is not NULL "
                          "and not Uint16Array");
        return false;
      }
      break;
    case GL_FLOAT_32_UNSIGNED_INT_24_8_REV:
      SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                        "type FLOAT_32_UNSIGNED_INT_24_8_REV but "
                        "ArrayBufferView is not NULL");
      return false;
    default:
      NOTREACHED();
  }

  unsigned total_bytes_required, skip_bytes;
  GLenum error = WebGLImageConversion::ComputeImageSizeInBytes(
      format, type, width, height, depth,
      GetUnpackPixelStoreParams(tex_dimension), &total_bytes_required, nullptr,
      &skip_bytes);
  if (error != GL_NO_ERROR) {
    SynthesizeGLError(error, function_name, "invalid texture dimensions");
    return false;
  }
  CheckedNumeric<uint32_t> total = src_offset;
  total *= pixels->TypeSize();
  total += total_bytes_required;
  total += skip_bytes;
  if (!total.IsValid() || pixels->byteLength() < total.ValueOrDie()) {
    SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                      "ArrayBufferView not big enough for request");
    return false;
  }
  return true;
}
