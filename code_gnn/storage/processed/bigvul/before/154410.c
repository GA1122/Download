BackTexture::~BackTexture() {
  DCHECK_EQ(id(), 0u);
  DCHECK(!image_.get());
}
