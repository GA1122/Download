IRCView::IRCView(QWidget* parent) : KTextBrowser(parent), m_rememberLine(0), m_lastMarkerLine(0), m_rememberLineDirtyBit(false), markerFormatObject(this)
{
    m_mousePressedOnUrl = false;
    m_isOnNick = false;
    m_isOnChannel = false;
    m_chatWin = 0;
    m_server = 0;

    setAcceptDrops(false);

    connect(document(), SIGNAL(contentsChange(int,int,int)), SLOT(cullMarkedLine(int,int,int)));

    QTextObjectInterface *iface = qobject_cast<QTextObjectInterface *>(&markerFormatObject);
    if (!iface)
    {
        Q_ASSERT(iface);
    }

    document()->documentLayout()->registerHandler(IRCView::MarkerLine, &markerFormatObject);
    document()->documentLayout()->registerHandler(IRCView::RememberLine, &markerFormatObject);


    connect(this, SIGNAL(anchorClicked(QUrl)), this, SLOT(anchorClicked(QUrl)));
    connect( this, SIGNAL(highlighted(QString)), this, SLOT(highlightedSlot(QString)) );
    setOpenLinks(false);
    setUndoRedoEnabled(0);
    document()->setDefaultStyleSheet("a.nick:link {text-decoration: none}");
    setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    setFocusPolicy(Qt::ClickFocus);
    setReadOnly(true);
    viewport()->setCursor(Qt::ArrowCursor);
    setTextInteractionFlags(Qt::TextBrowserInteraction);
    viewport()->setMouseTracking(true);

    if (Preferences::self()->useParagraphSpacing()) enableParagraphSpacing();

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setContextMenuOptions(IrcContextMenus::ShowTitle | IrcContextMenus::ShowFindAction, true);
}
