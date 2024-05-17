void WebLocalFrameImpl::RemoveSpellingMarkersUnderWords(
    const WebVector<WebString>& words) {
  Vector<String> converted_words;
  converted_words.Append(words.Data(), words.size());
  GetFrame()->RemoveSpellingMarkersUnderWords(converted_words);
}
