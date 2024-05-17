void WebFrame::setFrameOwnerProperties(const WebFrameOwnerProperties& properties)
{
    RemoteFrameOwner* owner = toRemoteFrameOwner(toImplBase()->frame()->owner());
    DCHECK(owner);
    owner->setScrollingMode(properties.scrollingMode);
    owner->setMarginWidth(properties.marginWidth);
    owner->setMarginHeight(properties.marginHeight);
    owner->setAllowFullscreen(properties.allowFullscreen);
}
