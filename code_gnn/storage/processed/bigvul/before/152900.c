bool ImageBitmap::isSourceSizeValid(int sourceWidth,
                                    int sourceHeight,
                                    ExceptionState& exceptionState) {
  if (!sourceWidth || !sourceHeight) {
    exceptionState.throwDOMException(
        IndexSizeError, String::format("The source %s provided is 0.",
                                       sourceWidth ? "height" : "width"));
    return false;
  }
  return true;
}
