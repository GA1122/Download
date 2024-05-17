bool Extension::UpdatesFromGallery() const {
  return update_url() == GalleryUpdateUrl(false) ||
         update_url() == GalleryUpdateUrl(true);
}
