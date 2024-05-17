WebKitTestResultPrinter::WebKitTestResultPrinter(
    std::ostream* output, std::ostream* error)
    : state_(DURING_TEST),
      capture_text_only_(false),
      encode_binary_data_(false),
      output_(output),
      error_(error) {
}
