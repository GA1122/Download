  void Run() {
    content::GetContentClient()->browser()->GetClipboard()->WriteObjects(
        *objects_.get());
  }
