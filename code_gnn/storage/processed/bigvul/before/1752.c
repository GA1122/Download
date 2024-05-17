int IRCView::defaultHtmlReplace(QString& htmlText, TextHtmlData* data, int pos, const QString& tag)
{
    QString replace;
    if (data->openHtmlTags.contains(tag))
    {
        replace = closeToTagString(data, tag);
    }
    else
    {
        data->openHtmlTags.append(tag);
        replace = QLatin1Char('<') + tag + QLatin1Char('>');
    }
    htmlText.replace(pos, 1, replace);
    return replace.length();
}
