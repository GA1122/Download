NOINLINE void ClassicPendingScript::CheckState() const {
  CHECK(!prefinalizer_called_);
  CHECK(GetElement());
  CHECK_EQ(is_external_, !!GetResource());
  CHECK(GetResource() || !streamer_);
}
