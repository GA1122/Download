void SecurityExploitBrowserTest::TestFileChooserWithPath(
    const base::FilePath& path) {
  GURL foo("http://foo.com/simple_page.html");
  NavigateToURL(shell(), foo);
  EXPECT_EQ(base::ASCIIToUTF16("OK"), shell()->web_contents()->GetTitle());

  RenderViewHost* compromised_renderer =
      shell()->web_contents()->GetRenderViewHost();
  RenderProcessHostWatcher terminated(
      shell()->web_contents(),
      RenderProcessHostWatcher::WATCH_FOR_PROCESS_EXIT);

  FileChooserParams params;
  params.default_file_name = path;

  ViewHostMsg_RunFileChooser evil(compromised_renderer->GetRoutingID(), params);

  IpcSecurityTestUtil::PwnMessageReceived(
      compromised_renderer->GetProcess()->GetChannel(), evil);
  terminated.Wait();
}
