void BookmarkNodeData::Write(Profile* profile, ui::OSExchangeData* data) const {
  DCHECK(data);

  if (elements.size() == 1 && elements[0].is_url) {
    if (elements[0].url.SchemeIs(content::kJavaScriptScheme)) {
      data->SetString(base::UTF8ToUTF16(elements[0].url.spec()));
    } else {
      data->SetURL(elements[0].url, elements[0].title);
    }
  }

  Pickle data_pickle;
  WriteToPickle(profile, &data_pickle);

  data->SetPickledData(GetBookmarkCustomFormat(), data_pickle);
}
