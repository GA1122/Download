FloatPoint WebPagePrivate::centerOfVisibleContentsRect() const
{
    FloatRect visibleContentsRect = this->visibleContentsRect();

    return FloatPoint(visibleContentsRect.x() + visibleContentsRect.width() / 2.0,
                      visibleContentsRect.y() + visibleContentsRect.height() / 2.0);
}
