  void ShowUploadBubble(bool should_cvc_be_requested = false) {
    SetLegalMessage(
        "{"
        "  \"line\" : [ {"
        "     \"template\": \"This is the entire message.\""
        "  } ]"
        "}",
        should_cvc_be_requested);
  }
