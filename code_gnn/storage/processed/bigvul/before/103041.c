  void LoadURL(TabContents* con, const std::wstring& url) {
    controller().LoadURL(GURL(WideToUTF16(url)), GURL(), PageTransition::LINK);
  }
