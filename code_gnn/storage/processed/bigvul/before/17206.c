scoped_refptr<BrowserContextDelegate> BrowserContextIOData::GetDelegate() {
  BrowserContextSharedIOData& data = GetSharedData();
  base::AutoLock lock(data.lock);
  return data.delegate;
}
