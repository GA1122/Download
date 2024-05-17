bool WebGLRenderingContextBase::ValidateReadPixelsFormatAndType(
    GLenum format,
    GLenum type,
    DOMArrayBufferView* buffer) {
  switch (format) {
    case GL_ALPHA:
    case GL_RGB:
    case GL_RGBA:
      break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "readPixels", "invalid format");
      return false;
  }

  switch (type) {
    case GL_UNSIGNED_BYTE:
      if (buffer && buffer->GetType() != DOMArrayBufferView::kTypeUint8) {
        SynthesizeGLError(
            GL_INVALID_OPERATION, "readPixels",
            "type UNSIGNED_BYTE but ArrayBufferView not Uint8Array");
        return false;
      }
      return true;
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_5_5_5_1:
      if (buffer && buffer->GetType() != DOMArrayBufferView::kTypeUint16) {
        SynthesizeGLError(
            GL_INVALID_OPERATION, "readPixels",
            "type UNSIGNED_SHORT but ArrayBufferView not Uint16Array");
        return false;
      }
      return true;
    case GL_FLOAT:
      if (ExtensionEnabled(kOESTextureFloatName) ||
          ExtensionEnabled(kOESTextureHalfFloatName)) {
        if (buffer && buffer->GetType() != DOMArrayBufferView::kTypeFloat32) {
          SynthesizeGLError(GL_INVALID_OPERATION, "readPixels",
                            "type FLOAT but ArrayBufferView not Float32Array");
          return false;
        }
        return true;
      }
      SynthesizeGLError(GL_INVALID_ENUM, "readPixels", "invalid type");
      return false;
    case GL_HALF_FLOAT_OES:
      if (ExtensionEnabled(kOESTextureHalfFloatName)) {
        if (buffer && buffer->GetType() != DOMArrayBufferView::kTypeUint16) {
          SynthesizeGLError(
              GL_INVALID_OPERATION, "readPixels",
              "type HALF_FLOAT_OES but ArrayBufferView not Uint16Array");
          return false;
        }
        return true;
      }
      SynthesizeGLError(GL_INVALID_ENUM, "readPixels", "invalid type");
      return false;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "readPixels", "invalid type");
      return false;
  }
}
