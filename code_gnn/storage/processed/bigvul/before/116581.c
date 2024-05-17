  virtual void SetUp() {
    content::GetContentClient()->set_renderer(&mock_content_renderer_client_);

    static const char kThreadName[] = "RenderThread";
    channel_.reset(new IPC::Channel(kThreadName,
        IPC::Channel::MODE_SERVER, this));
    ASSERT_TRUE(channel_->Connect());

    mock_process_.reset(new MockRenderProcess);
    render_thread_ = new RenderThreadImpl(kThreadName);

    decoder_ = new media::MockAudioDecoder();

    EXPECT_CALL(*decoder_, bits_per_channel())
        .WillRepeatedly(Return(16));
    EXPECT_CALL(*decoder_, channel_layout())
        .WillRepeatedly(Return(CHANNEL_LAYOUT_MONO));
    EXPECT_CALL(*decoder_, samples_per_second())
        .WillRepeatedly(Return(44100));

    scoped_refptr<media::AudioRendererSink> default_sink =
        new AudioDevice();

    renderer_ = new TestAudioRendererImpl(default_sink.get());
    renderer_->Initialize(decoder_,
                          media::NewExpectedStatusCB(media::PIPELINE_OK),
                          NewUnderflowClosure(), NewAudioTimeClosure());

    event_.reset(new base::WaitableEvent(false, false));
  }
