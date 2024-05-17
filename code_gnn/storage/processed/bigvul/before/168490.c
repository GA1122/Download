  WebAssociatedURLLoaderTest()
      : will_follow_redirect_(false),
        did_send_data_(false),
        did_receive_response_(false),
        did_receive_data_(false),
        did_receive_cached_metadata_(false),
        did_finish_loading_(false),
        did_fail_(false) {
    frame_file_path_ = test::CoreTestDataPath("iframes_test.html");
  }
