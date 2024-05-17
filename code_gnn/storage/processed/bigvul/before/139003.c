MovableOnDestroyCallback::~MovableOnDestroyCallback() {
  if (!callback_.is_null())
    callback_.Run();
}
