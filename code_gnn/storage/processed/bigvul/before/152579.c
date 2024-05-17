  void OnRunJavaScriptDialog(const base::string16& message,
                             const base::string16&,
                             JavaScriptDialogType,
                             bool*,
                             base::string16*) {
    callback_.Run(message);
  }
