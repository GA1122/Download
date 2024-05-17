  void SetResponseStringAndFetch(const std::string& selected_text,
                                 const std::string& mentions_start,
                                 const std::string& mentions_end) {
    fetcher()->set_response_code(200);
      fetcher()->SetResponseString(
          ")]}'\n"
