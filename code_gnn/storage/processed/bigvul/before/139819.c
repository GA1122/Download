Shell* Shell::GetInstance() {
  CHECK(instance_);
  return instance_;
}
