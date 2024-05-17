  void SplitUrlAndTitle(const base::string16& str,
                      base::string16* url,
//                       GURL* url,
                        base::string16* title) {
    DCHECK(url && title);
    size_t newline_pos = str.find('\n');
    if (newline_pos != base::string16::npos) {
    url->assign(str, 0, newline_pos);
//     *url = GURL(base::string16(str, 0, newline_pos));
      title->assign(str, newline_pos + 1, base::string16::npos);
    } else {
    url->assign(str);
//     *url = GURL(str);
      title->assign(str);
    }
  }