QString IRCView::ircTextToHtml(const QString& text, bool parseURL, const QString& defaultColor,
                               const QString& whoSent, bool closeAllTags, QChar::Direction* direction)
{
    TextHtmlData data;
    data.defaultColor = defaultColor;
    QString htmlText(text);

    bool allowColors = Preferences::self()->allowColorCodes();
    QString linkColor = Preferences::self()->color(Preferences::Hyperlink).name();

    unsigned int rtl_chars = 0;
    unsigned int ltr_chars = 0;

    QString fromNick;
    TextUrlData urlData;
    TextChannelData channelData;
    if (parseURL)
    {
        QString strippedText(removeIrcMarkup(htmlText));
        urlData = extractUrlData(strippedText);
        if (!urlData.urlRanges.isEmpty())
        {
            adjustUrlRanges(urlData.urlRanges, urlData.fixedUrls, htmlText, strippedText);

            if (whoSent.isEmpty())
                fromNick = m_chatWin->getName();
            else
                fromNick = whoSent;
        }

        channelData = extractChannelData(strippedText);
        adjustUrlRanges(channelData.channelRanges, channelData.fixedChannels , htmlText, strippedText);
    }
    else
    {
        htmlText.replace('&', "&amp;");
        htmlText.replace("\x0b", "&");
    }

    int linkPos = -1;
    int linkOffset = 0;
    bool doChannel = false;
    if (parseURL)
    {
        if (!urlData.urlRanges.isEmpty() && !channelData.channelRanges.isEmpty())
        {
            if (urlData.urlRanges.first() < channelData.channelRanges.first())
            {
                doChannel = false;
                linkPos = urlData.urlRanges.first().first;
            }
            else
            {
                doChannel = true;
                linkPos = channelData.channelRanges.first().first;
            }
        }
        else if (!urlData.urlRanges.isEmpty() && channelData.channelRanges.isEmpty())
        {
            doChannel = false;
            linkPos = urlData.urlRanges.first().first;
        }
        else if (urlData.urlRanges.isEmpty() && !channelData.channelRanges.isEmpty())
        {
            doChannel = true;
            linkPos = channelData.channelRanges.first().first;
        }
        else
        {
            linkPos = -1;
        }
    }

    QChar lastChar;
    int offset;
    for (int pos = 0; pos < htmlText.length(); ++pos)
    {
        if (parseURL && pos == linkPos+linkOffset)
        {
            if (doChannel)
            {
                QString fixedChannel = channelData.fixedChannels.takeFirst();
                const QPair<int, int>& range = channelData.channelRanges.takeFirst();

                QString oldChannel = htmlText.mid(pos, range.second);
                QString strippedChannel = removeIrcMarkup(oldChannel);
                QString colorCodes = extractColorCodes(oldChannel);

                QString link("%1<a href=\"#%2\" style=\"color:" + linkColor + "\">%3</a>%4%5");

                link = link.arg(closeTags(&data), fixedChannel, strippedChannel, openTags(&data, 0), colorCodes);
                htmlText.replace(pos, oldChannel.length(), link);

                pos += link.length() - colorCodes.length() - 1;
                linkOffset += link.length() - oldChannel.length();
            }
            else
            {
                QString fixedUrl = urlData.fixedUrls.takeFirst();
                const QPair<int, int>& range = urlData.urlRanges.takeFirst();

                QString oldUrl = htmlText.mid(pos, range.second);
                QString strippedUrl = removeIrcMarkup(oldUrl);

                QString closeTagsString(closeTags(&data));
                QString colorCodes = extractColorCodes(oldUrl);
                colorCodes = removeDuplicateCodes(colorCodes, &data, allowColors);

                QString link("%1<a href=\"%2\" style=\"color:" + linkColor + "\">%3</a>%4%5");

                link = link.arg(closeTagsString, fixedUrl, strippedUrl, openTags(&data, 0), colorCodes);
                htmlText.replace(pos, oldUrl.length(), link);

                QMetaObject::invokeMethod(Application::instance(), "storeUrl", Qt::QueuedConnection,
                                          Q_ARG(QString, fromNick), Q_ARG(QString, fixedUrl), Q_ARG(QDateTime, QDateTime::currentDateTime()));

                pos += link.length() - colorCodes.length() - 1;
                linkOffset += link.length() - oldUrl.length();
            }

            bool invalidNextLink = false;
            do
            {

                if (!urlData.urlRanges.isEmpty() && !channelData.channelRanges.isEmpty())
                {
                    if (urlData.urlRanges.first() < channelData.channelRanges.first())
                    {
                        doChannel = false;
                        linkPos = urlData.urlRanges.first().first;
                    }
                    else
                    {
                        doChannel = true;
                        linkPos = channelData.channelRanges.first().first;
                    }
                }
                else if (!urlData.urlRanges.isEmpty() && channelData.channelRanges.isEmpty())
                {
                    doChannel = false;
                    linkPos = urlData.urlRanges.first().first;
                }
                else if (urlData.urlRanges.isEmpty() && !channelData.channelRanges.isEmpty())
                {
                    doChannel = true;
                    linkPos = channelData.channelRanges.first().first;
                }
                else
                {
                    linkPos = -1;
                }

                if (linkPos > -1 && linkPos+linkOffset < pos)
                {
                    invalidNextLink = true;
                    if (doChannel)
                    {
                        channelData.channelRanges.removeFirst();
                        channelData.fixedChannels.removeFirst();
                    }
                    else
                    {
                        urlData.urlRanges.removeFirst();
                        urlData.fixedUrls.removeFirst();
                    }
                }
                else
                {
                    invalidNextLink = false;
                }
            } while (invalidNextLink);

            continue;
        }


        switch (htmlText.at(pos).toAscii())
        {
            case '\x02':  
                offset = defaultHtmlReplace(htmlText, &data, pos, QLatin1String("b"));
                pos += offset -1;
                linkOffset += offset -1;
                break;
            case '\x1d':  
                offset = defaultHtmlReplace(htmlText, &data, pos, QLatin1String("i"));
                pos += offset -1;
                linkOffset += offset -1;
                break;
            case '\x15':  
            case '\x1f':  
                offset = defaultHtmlReplace(htmlText, &data, pos, QLatin1String("u"));
                pos += offset -1;
                linkOffset += offset -1;
                break;
            case '\x13':  
                offset = defaultHtmlReplace(htmlText, &data, pos, QLatin1String("s"));
                pos += offset -1;
                linkOffset += offset -1;
                break;
            case '\x03':  
                {
                    QString fgColor, bgColor;
                    bool fgOK = true, bgOK = true;
                    QString colorMatch(getColors(htmlText, pos, fgColor, bgColor, &fgOK, &bgOK));

                    if (!allowColors)
                    {
                        htmlText.remove(pos, colorMatch.length());
                        pos -= 1;
                        linkOffset -= colorMatch.length();
                        break;
                    }
                    QString colorString;
                    if (colorMatch == QLatin1String("\x03") || colorMatch == QLatin1String("\x11") ||
                        (fgColor.isEmpty() && bgColor.isEmpty()) || (!fgOK && !bgOK))
                    {
                        if (data.reverse)
                        {
                            data.lastFgColor.clear();
                            data.lastBgColor.clear();
                        }
                        else
                        {
                            if (data.openHtmlTags.contains(QLatin1String("font")) &&
                                data.openHtmlTags.contains(QLatin1String("span")))
                            {
                                colorString += closeToTagString(&data, QLatin1String("span"));
                                data.lastBgColor.clear();
                                colorString += closeToTagString(&data, QLatin1String("font"));
                                data.lastFgColor.clear();
                            }
                            else if (data.openHtmlTags.contains("font"))
                            {
                                colorString += closeToTagString(&data, QLatin1String("font"));
                                data.lastFgColor.clear();
                            }
                        }
                        htmlText.replace(pos, colorMatch.length(), colorString);

                        pos += colorString.length() - 1;
                        linkOffset += colorString.length() -colorMatch.length();
                        break;
                    }

                    if (!fgOK)
                    {
                        fgColor = defaultColor;
                    }
                    if (!bgOK)
                    {
                        bgColor = fontColorOpenTag(Preferences::self()->color(Preferences::TextViewBackground).name());
                    }

                    if (data.reverse)
                    {
                        if (!fgColor.isEmpty())
                        {
                            data.lastFgColor = fgColor;
                            if (!bgColor.isEmpty())
                            {
                                data.lastBgColor = bgColor;
                            }
                        }
                    }
                    else if (!fgColor.isEmpty())
                    {
                        if (data.openHtmlTags.contains(QLatin1String("font")) &&
                            data.openHtmlTags.contains(QLatin1String("span")))
                        {
                            colorString += closeToTagString(&data, QLatin1String("span"));
                            colorString += closeToTagString(&data, QLatin1String("font"));
                        }
                        else if (data.openHtmlTags.contains(QLatin1String("font")))
                        {
                            colorString += closeToTagString(&data, QLatin1String("font"));
                        }
                        data.lastFgColor = fgColor;
                        if (!bgColor.isEmpty())
                            data.lastBgColor = bgColor;

                        if (!data.lastFgColor.isEmpty())
                        {
                            colorString += fontColorOpenTag(data.lastFgColor);
                            data.openHtmlTags.append(QLatin1String("font"));
                            if (!data.lastBgColor.isEmpty())
                            {
                                colorString += spanColorOpenTag(data.lastBgColor);
                                data.openHtmlTags.append(QLatin1String("span"));
                            }
                        }
                    }
                    htmlText.replace(pos, colorMatch.length(), colorString);

                    pos += colorString.length() - 1;
                    linkOffset += colorString.length() -colorMatch.length();
                    break;
                }
                break;
            case '\x0f':  
                {
                    QString closeText;
                    while (!data.openHtmlTags.isEmpty())
                    {
                        closeText += QLatin1String("</") + data.openHtmlTags.takeLast() + QLatin1Char('>');
                    }
                    data.lastBgColor.clear();
                    data.lastFgColor.clear();
                    data.reverse = false;
                    htmlText.replace(pos, 1, closeText);

                    pos += closeText.length() - 1;
                    linkOffset += closeText.length() - 1;
                }
                break;
            case '\x16':  
                {
                    if (!allowColors)
                    {
                        htmlText.remove(pos, 1);
                        pos -= 1;
                        linkOffset -= 1;
                        break;
                    }

                    QString colorString;

                    if (!data.reverse)
                    {
                        if (data.openHtmlTags.contains(QLatin1String("span")))
                        {
                            colorString += closeToTagString(&data, QLatin1String("span"));
                        }
                        if (data.openHtmlTags.contains(QLatin1String("font")))
                        {
                            colorString += closeToTagString(&data, QLatin1String("font"));
                        }
                        data.reverse = true;
                        colorString += fontColorOpenTag(Preferences::self()->color(Preferences::TextViewBackground).name());
                        data.openHtmlTags.append(QLatin1String("font"));
                        colorString += spanColorOpenTag(defaultColor);
                        data.openHtmlTags.append(QLatin1String("span"));
                    }
                    else
                    {
                        colorString += closeToTagString(&data, QLatin1String("span"));
                        colorString += closeToTagString(&data, QLatin1String("font"));
                        data.reverse = false;
                        if (!data.lastFgColor.isEmpty())
                        {
                            colorString += fontColorOpenTag(data.lastFgColor);
                            data.openHtmlTags.append(QLatin1String("font"));
                            if (!data.lastBgColor.isEmpty())
                            {
                                colorString += spanColorOpenTag(data.lastBgColor);
                                data.openHtmlTags.append(QLatin1String("span"));
                            }
                        }
                    }
                    htmlText.replace(pos, 1, colorString);
                    pos += colorString.length() -1;
                    linkOffset += colorString.length() -1;
                }
                break;
            default:
                {
                    const QChar& dirChar = htmlText.at(pos);

                    if (dirChar == ' ' &&
                        !lastChar.isNull() && lastChar == ' ')
                    {
                        htmlText[pos] = '\xA0';
                        lastChar = '\xA0';
                    }
                    else
                    {
                        lastChar = dirChar;
                    }

                    if (!(dirChar.isNumber() || dirChar.isSymbol() ||
                        dirChar.isSpace()  || dirChar.isPunct()  ||
                        dirChar.isMark()))
                    {
                        switch(dirChar.direction())
                        {
                            case QChar::DirL:
                            case QChar::DirLRO:
                            case QChar::DirLRE:
                                ltr_chars++;
                                break;
                            case QChar::DirR:
                            case QChar::DirAL:
                            case QChar::DirRLO:
                            case QChar::DirRLE:
                                rtl_chars++;
                                break;
                            default:
                                break;
                        }
                    }
                }
        }
    }

    if (direction)
    {
        if (rtl_chars > ltr_chars)
            *direction = QChar::DirR;
        else
            *direction = QChar::DirL;
    }

    if (parseURL)
    {
        htmlText.replace('&', "&amp;");
        htmlText.replace("\x0b", "&");
    }

    if (closeAllTags)
    {
        htmlText += closeTags(&data);
    }

    return htmlText;
}
