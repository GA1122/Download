String urlToMarkup(const KURL& url, const String& title)
{
    StringBuilder markup;
    markup.append("<a href=\"");
    markup.append(url.string());
    markup.append("\">");
    appendCharactersReplacingEntities(markup, title.characters(), title.length(), EntityMaskInPCDATA);
    markup.append("</a>");
     return markup.toString();
 }
