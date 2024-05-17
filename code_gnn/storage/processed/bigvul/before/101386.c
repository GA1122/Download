GURL BaseNode::GetURL() const {
  return GURL(GetBookmarkSpecifics().url());
}
