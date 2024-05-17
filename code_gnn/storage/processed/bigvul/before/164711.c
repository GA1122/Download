void SecurityExploitBrowserTest::TestFileChooserWithPath(
    const base::FilePath& path) {
  GURL foo("http://foo.com/simple_page.html");
  NavigateToURL(shell(), foo);
  EXPECT_EQ(base::ASCIIToUTF16("OK"), shell()->web_contents()->GetTitle());

  RenderFrameHost* compromised_renderer =
      shell()->web_contents()->GetMainFrame();
  blink::mojom::FileChooserParamsPtr params =
      blink::mojom::FileChooserParams::New();
  params->default_file_name = path;

  mojo::test::BadMessageObserver bad_message_observer;
  blink::mojom::FileChooserPtr factory =
      static_cast<RenderFrameHostImpl*>(compromised_renderer)
          ->BindFileChooserForTesting();
  factory->OpenFileChooser(
      std::move(params), blink::mojom::FileChooser::OpenFileChooserCallback());
  factory.FlushForTesting();
  EXPECT_THAT(bad_message_observer.WaitForBadMessage(),
              ::testing::StartsWith("FileChooser: The default file name"));
}
