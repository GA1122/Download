void CheckDisplayModeMQ(
    const base::string16& display_mode,
    content::WebContents* web_contents) {
  base::string16 funtcion =
      ASCIIToUTF16("(function() {return window.matchMedia('(display-mode: ") +
      display_mode + ASCIIToUTF16(")').matches;})();");
  JSBooleanResultGetter js_result_getter;
  base::RunLoop run_loop;
  web_contents->GetMainFrame()->ExecuteJavaScriptForTests(
      funtcion,
      base::Bind(&JSBooleanResultGetter::OnJsExecutionDone,
          base::Unretained(&js_result_getter), run_loop.QuitClosure()));
  run_loop.Run();
  EXPECT_TRUE(js_result_getter.GetResult());
}
