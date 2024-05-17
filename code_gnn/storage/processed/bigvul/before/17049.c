OxideQQuickWebView::OxideQQuickWebView(QQuickItem* parent)
    : QQuickItem(parent),
      d_ptr(new OxideQQuickWebViewPrivate(this)) {
  oxide::qquick::EnsureChromiumStarted();

  Q_D(OxideQQuickWebView);

  setFlags(QQuickItem::ItemClipsChildrenToShape |
           QQuickItem::ItemHasContents |
           QQuickItem::ItemIsFocusScope |
           QQuickItem::ItemAcceptsDrops);
  setAcceptedMouseButtons(Qt::AllButtons);
  setAcceptHoverEvents(true);
}
