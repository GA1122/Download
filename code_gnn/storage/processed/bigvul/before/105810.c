void Label::SetURL(const GURL& url) {
  url_ = url;
  text_ = UTF8ToUTF16(url_.spec());
  url_set_ = true;
  text_size_valid_ = false;
  PreferredSizeChanged();
  SchedulePaint();
}
