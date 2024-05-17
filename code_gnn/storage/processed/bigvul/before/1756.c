void IRCView::dragMoveEvent(QDragMoveEvent* e)
{
    if (e->mimeData()->hasUrls())
        e->accept();
    else
        e->ignore();
}
