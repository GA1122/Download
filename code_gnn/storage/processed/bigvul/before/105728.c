void FilterInputMethods(const std::vector<std::string>& requested_input_methods,
                        std::vector<std::string>* out_filtered_input_methods) {
  out_filtered_input_methods->clear();
  for (size_t i = 0; i < requested_input_methods.size(); ++i) {
    const std::string& input_method = requested_input_methods[i];
    if (InputMethodIdIsWhitelisted(input_method.c_str())) {
      out_filtered_input_methods->push_back(input_method);
    } else {
      LOG(ERROR) << "Unsupported input method: " << input_method;
    }
  }
}
