std::vector<WebContentsImpl*> WebContentsImpl::GetWebContentsAndAllInner() {
  std::vector<WebContentsImpl*> all_contents(1, this);

  for (size_t i = 0; i != all_contents.size(); ++i) {
    for (auto* inner_contents : all_contents[i]->GetInnerWebContents()) {
      all_contents.push_back(inner_contents);
    }
  }

  return all_contents;
}
