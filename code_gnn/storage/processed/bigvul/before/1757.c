void IrcViewMarkerLine::drawObject(QPainter *painter, const QRectF &r, QTextDocument *doc, int posInDocument, const QTextFormat &format)
{
    Q_UNUSED(format);

    QTextBlock block=doc->findBlock(posInDocument);
    QPen pen;
    Burr* b = dynamic_cast<Burr*>(block.userData());
    Q_ASSERT(b);  
    switch (b->m_format)
    {
        case IRCView::BlockIsMarker:
            pen.setColor(Preferences::self()->color(Preferences::ActionMessage));
            break;

        case IRCView::BlockIsRemember:
            pen.setColor(Preferences::self()->color(Preferences::CommandMessage));
            break;

        default:
            pen.setColor(Qt::cyan);
    }

    pen.setWidth(2);  
    painter->setPen(pen);

    qreal y = (r.top() + r.height() / 2);
    QLineF line(r.left(), y, r.right(), y);
    painter->drawLine(line);
}
