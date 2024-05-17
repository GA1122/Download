GURL CreateBlobURL(content::RenderFrameHost* frame,
                   const std::string& content) {
  std::string blob_url_string;
  EXPECT_TRUE(ExecuteScriptAndExtractString(
      frame,
      "var blob = new Blob(['<html><body>" + content + "</body></html>'],\n"
      "                    {type: 'text/html'});\n"
      "domAutomationController.send(URL.createObjectURL(blob));\n",
      &blob_url_string));
  GURL blob_url(blob_url_string);
  EXPECT_TRUE(blob_url.is_valid());
  EXPECT_TRUE(blob_url.SchemeIsBlob());
  return blob_url;
}
