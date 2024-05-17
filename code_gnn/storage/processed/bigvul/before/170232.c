bool IsJavascriptEnabled(content::WebContents* contents) {
  std::unique_ptr<base::Value> value =
      content::ExecuteScriptAndGetValue(contents->GetMainFrame(), "123");
  int result = 0;
  if (!value->GetAsInteger(&result))
    EXPECT_EQ(base::Value::Type::NONE, value->type());
  return result == 123;
}
