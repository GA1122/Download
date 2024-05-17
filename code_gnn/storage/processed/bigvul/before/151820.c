void BackgroundLoaderOfflinerTest::OnCancel(const SavePageRequest& request) {
  DCHECK(!cancel_callback_called_);
  cancel_callback_called_ = true;
}
