bool BrowserContextIOData::GetDoNotTrack() const {
  const BrowserContextSharedIOData& data = GetSharedData();
  base::AutoLock lock(data.lock);
  return data.do_not_track;
}
