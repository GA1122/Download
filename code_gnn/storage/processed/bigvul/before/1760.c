QString IRCView::filter(const QString& line, const QString& defaultColor, const QString& whoSent,
    bool doHighlight, bool parseURL, bool self, QChar::Direction* direction)
{
    QString filteredLine(line);
    Application* konvApp = static_cast<Application*>(kapp);


    if (filteredLine[0] == ' ')
    {
        filteredLine[0] = '\xA0';
    }

    filteredLine.replace('<', "\x0blt;");
    filteredLine.replace('>', "\x0bgt;");

    if (filteredLine.contains('\x07'))
    {
        if (Preferences::self()->beep())
        {
            kapp->beep();
        }
        filteredLine.remove('\x07');
    }

    filteredLine = ircTextToHtml(filteredLine,  parseURL, defaultColor, whoSent, true, direction);

    QString ownNick;

    if (m_server)
    {
        ownNick = m_server->getNickname();
    }
    else if (m_chatWin->getType() == ChatWindow::DccChat)
    {
        ownNick = static_cast<DCC::ChatContainer*>(m_chatWin)->ownNick();
    }

    if(doHighlight && (whoSent != ownNick) && !self)
    {
        QString highlightColor;

        if (Preferences::self()->highlightNick() &&
            line.toLower().contains(QRegExp("(^|[^\\d\\w])" +
            QRegExp::escape(ownNick.toLower()) +
            "([^\\d\\w]|$)")))
        {
            highlightColor = Preferences::self()->highlightNickColor().name();
            m_tabNotification = Konversation::tnfNick;
        }
        else
        {
            QList<Highlight*> highlightList = Preferences::highlightList();
            QListIterator<Highlight*> it(highlightList);
            Highlight* highlight;
            QStringList highlightChatWindowList;
            bool patternFound = false;

            QStringList captures;
            while (it.hasNext())
            {
                highlight = it.next();
                QStringList highlightChatWindowList = highlight->getChatWindowList();
                if (highlightChatWindowList.isEmpty() ||
                    highlightChatWindowList.contains(m_chatWin->getName(), Qt::CaseInsensitive))
                {
                    if (highlight->getRegExp())
                    {
                        QRegExp needleReg(highlight->getPattern());
                        needleReg.setCaseSensitivity(Qt::CaseInsensitive);
                        patternFound = ((line.contains(needleReg)) ||
                            (whoSent.contains(needleReg)));

                        captures = needleReg.capturedTexts();

                    }
                    else
                    {
                        QString needle = highlight->getPattern();
                        patternFound = ((line.contains(needle, Qt::CaseInsensitive)) ||
                            (whoSent.contains(needle, Qt::CaseInsensitive)));
                    }

                    if (patternFound)
                    {
                        break;
                    }
                }
            }

            if (patternFound)
            {
                highlightColor = highlight->getColor().name();
                m_highlightColor = highlightColor;
                m_tabNotification = Konversation::tnfHighlight;

                if (Preferences::self()->highlightSoundsEnabled() && m_chatWin->notificationsEnabled())
                {
                    konvApp->sound()->play(highlight->getSoundURL());
                }

                konvApp->notificationHandler()->highlight(m_chatWin, whoSent, line);
                m_autoTextToSend = highlight->getAutoText();

                for (int capture = 0; capture < captures.count(); capture++)
                {
                    m_autoTextToSend.replace(QString("%%1").arg(capture), captures[capture]);
                }
                m_autoTextToSend.remove(QRegExp("%[0-9]"));
            }
        }

        if (!highlightColor.isEmpty())
        {
            filteredLine = QLatin1String("<font color=\"") + highlightColor + QLatin1String("\">") + filteredLine +
                QLatin1String("</font>");
        }
    }
    else if (doHighlight && (whoSent == ownNick) && Preferences::self()->highlightOwnLines())
    {
        filteredLine = QLatin1String("<font color=\"") + Preferences::self()->highlightOwnLinesColor().name() +
            QLatin1String("\">") + filteredLine + QLatin1String("</font>");
    }

    filteredLine = Konversation::Emoticons::parseEmoticons(filteredLine);

    return filteredLine;
}
