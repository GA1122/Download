static void CopyStringVector(
    const Vector<String>& input, WebVector<WebString>* output) {
  WebVector<WebString> result(input.size());
  for (size_t i = 0; i < input.size(); ++i)
    result[i] = webkit_glue::StringToWebString(input[i]);
  output->swap(result);
}
