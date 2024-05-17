  HTMLVideoElementCapturerSourceTest()
      : scoped_task_environment_(
            base::test::ScopedTaskEnvironment::MainThreadType::UI),
        web_media_player_(new MockWebMediaPlayer()),
        html_video_capturer_(new HtmlVideoElementCapturerSource(
            web_media_player_->AsWeakPtr(),
            blink::scheduler::GetSingleThreadTaskRunnerForTesting(),
            blink::scheduler::GetSingleThreadTaskRunnerForTesting())) {}
