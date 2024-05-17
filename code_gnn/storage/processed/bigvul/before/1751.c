void IRCView::cullMarkedLine(int where, int rem, int add)  
{
    bool showDebug = false;
    QString output;
    QDebug d = QDebug(&output); 

    bool merged = (add!=0 && rem !=0);  
    int blockCount = document()->blockCount();
    void *view = this;
    QTextBlock prime = document()->firstBlock();

    d << "================= cullMarkedLine" << _S(view) << _S(where) << _S(rem) << _S(add) << _S(blockCount) << _S(prime.length()) << _S(merged);

    if (prime.length() == 1)
    {
        if (document()->blockCount() == 1)  
        {
            d << "- wipeLineParagraphs()" << (void*)m_rememberLine << (void*)m_lastMarkerLine;
            wipeLineParagraphs();
        }
        else if (document()->characterAt(0).unicode() == 0x2029)
        {
            d << "- only QChar::ParagraphSeparator";
            if (dynamic_cast<Burr*>(prime.userData()))
                d << "Burr!" << prime.userData();
        }
        else
        {
            QString fc = "0x" + QString::number(document()->characterAt(0).unicode(), 16).rightJustified(4, '0');
            d << "- block of length 1 but not 2029" << qPrintable(fc);
        }
    }
    else if (prime.length() == 2)
    {
        QString fc = "0x" + QString::number(document()->characterAt(0).unicode(), 16).rightJustified(4, '0');
        QString sc = "0x" + QString::number(document()->characterAt(1).unicode(), 16).rightJustified(4, '0');
        d << "- prime(2)" << fc << sc;
    }
    if (showDebug)
    {
        DebugBanner;
        kDebug() << output;
    }
}
