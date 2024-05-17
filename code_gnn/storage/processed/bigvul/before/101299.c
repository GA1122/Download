void UpdateBookmarkSpecifics(const std::string& singleton_tag,
                             const std::string& url,
                             const std::string& favicon_bytes,
                             MutableEntry* local_entry) {
  if (singleton_tag == "google_chrome")
    return;
  sync_pb::EntitySpecifics pb;
  sync_pb::BookmarkSpecifics* bookmark = pb.MutableExtension(sync_pb::bookmark);
  if (!url.empty())
    bookmark->set_url(url);
  if (!favicon_bytes.empty())
    bookmark->set_favicon(favicon_bytes);
  local_entry->Put(SERVER_SPECIFICS, pb);
}
