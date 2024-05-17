void DownloadImageTestInternal(Shell* shell,
                               const GURL& image_url,
                               int expected_http_status,
                               int expected_number_of_images) {
  using ::testing::_;
  using ::testing::InvokeWithoutArgs;
  using ::testing::SizeIs;

  DownloadImageObserver download_image_observer;
  scoped_refptr<MessageLoopRunner> loop_runner =
      new MessageLoopRunner();

  EXPECT_CALL(download_image_observer,
              OnFinishDownloadImage(_, expected_http_status, _,
                                    SizeIs(expected_number_of_images), _));
  ON_CALL(download_image_observer, OnFinishDownloadImage(_, _, _, _, _))
      .WillByDefault(
          InvokeWithoutArgs(loop_runner.get(), &MessageLoopRunner::Quit));

  shell->LoadURL(GURL("about:blank"));
  shell->web_contents()->DownloadImage(
      image_url, false, 1024, false,
      base::Bind(&DownloadImageObserver::OnFinishDownloadImage,
                 base::Unretained(&download_image_observer)));

  loop_runner->Run();
}
