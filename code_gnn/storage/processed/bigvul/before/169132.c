GURL CreateFileSystemURL(content::RenderFrameHost* frame,
                         const std::string& content) {
  std::string filesystem_url_string;
  EXPECT_TRUE(ExecuteScriptAndExtractString(
      frame,
      "var blob = new Blob(['<html><body>" + content + "</body></html>'],\n"
      "                    {type: 'text/html'});\n"
      "window.webkitRequestFileSystem(TEMPORARY, blob.size, fs => {\n"
      "  fs.root.getFile('foo.html', {create: true}, file => {\n"
      "    file.createWriter(writer => {\n"
      "      writer.write(blob);\n"
      "      writer.onwriteend = () => {\n"
      "        domAutomationController.send(file.toURL());\n"
      "      }\n"
      "    });\n"
      "  });\n"
      "});\n",
      &filesystem_url_string));
  GURL filesystem_url(filesystem_url_string);
  EXPECT_TRUE(filesystem_url.is_valid());
  EXPECT_TRUE(filesystem_url.SchemeIsFileSystem());
  return filesystem_url;
}
