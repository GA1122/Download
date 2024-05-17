bool HTMLMediaElement::isMediaStreamURL(const String& url) {
  return s_mediaStreamRegistry ? s_mediaStreamRegistry->contains(url) : false;
}
