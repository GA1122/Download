std::string GetAllHeaders(const WebURLResponse& response) {
  std::string result;
  const WebString& status = response.httpStatusText();
  if (status.isEmpty())
    return result;

  result = StringPrintf("HTTP %d ", response.httpStatusCode());
  result.append(status.utf8());
  result.append("\n");

  HeaderFlattener flattener(&result);
  response.visitHTTPHeaderFields(&flattener);

  return result;
}
