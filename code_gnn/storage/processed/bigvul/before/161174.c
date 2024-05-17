void MediaStreamManager::UseFakeUIFactoryForTests(
    base::Callback<std::unique_ptr<FakeMediaStreamUIProxy>(void)>
        fake_ui_factory) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  fake_ui_factory_ = fake_ui_factory;
}
