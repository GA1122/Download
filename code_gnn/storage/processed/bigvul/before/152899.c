bool ImageBitmap::isResizeOptionValid(const ImageBitmapOptions& options,
                                      ExceptionState& exceptionState) {
  if ((options.hasResizeWidth() && options.resizeWidth() == 0) ||
      (options.hasResizeHeight() && options.resizeHeight() == 0)) {
    exceptionState.throwDOMException(
        InvalidStateError,
        "The resizeWidth or/and resizeHeight is equal to 0.");
    return false;
  }
  return true;
}
