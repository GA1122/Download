  void ExecuteJavaScriptForTests(const std::string& js) {
    base::RunLoop run_loop;
    browser()
        ->tab_strip_model()
        ->GetActiveWebContents()
        ->GetMainFrame()
        ->ExecuteJavaScriptForTests(
            base::ASCIIToUTF16(js),
            base::Bind([](const base::Closure& quit_callback,
                          const base::Value* result) { quit_callback.Run(); },
                       run_loop.QuitClosure()));
    run_loop.Run();
  }
