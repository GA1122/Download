void BlinkMediaTestSuite::Initialize() {
  base::TestSuite::Initialize();

#if defined(OS_ANDROID)
  JNIEnv* env = base::android::AttachCurrentThread();
  ui::gl::android::RegisterJni(env);
  media::RegisterJni(env);
#endif

  media::InitializeMediaLibrary();

#ifdef V8_USE_EXTERNAL_STARTUP_DATA
  gin::V8Initializer::LoadV8Snapshot();
  gin::V8Initializer::LoadV8Natives();
#endif

  scoped_ptr<base::MessageLoop> message_loop;
  if (!base::MessageLoop::current())
    message_loop.reset(new base::MessageLoop());
  blink::initialize(blink_platform_support_.get());
}
