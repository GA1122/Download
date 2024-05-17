QVariant QQuickWebView::inputMethodQuery(Qt::InputMethodQuery property) const
{
    Q_D(const QQuickWebView);
    const EditorState& state = d->webPageProxy->editorState();

    switch(property) {
    case Qt::ImCursorRectangle:
        return QRectF(state.cursorRect);
    case Qt::ImFont:
        return QVariant();
    case Qt::ImCursorPosition:
        return QVariant(static_cast<int>(state.cursorPosition));
    case Qt::ImAnchorPosition:
        return QVariant(static_cast<int>(state.anchorPosition));
    case Qt::ImSurroundingText:
        return QString(state.surroundingText);
    case Qt::ImCurrentSelection:
        return QString(state.selectedText);
    case Qt::ImMaximumTextLength:
        return QVariant();  
    case Qt::ImHints:
        return int(Qt::InputMethodHints(state.inputMethodHints));
    default:
        return QQuickFlickable::inputMethodQuery(property);
    }
}
