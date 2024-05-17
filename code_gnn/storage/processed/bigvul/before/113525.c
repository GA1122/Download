Platform::WebContext WebPagePrivate::webContext(TargetDetectionStrategy strategy)
{
    Platform::WebContext context;

    RefPtr<Node> node = contextNode(strategy);
    m_currentContextNode = node;
    if (!m_currentContextNode)
        return context;

    PlatformMouseEvent mouseEvent(IntPoint(), IntPoint(), PlatformEvent::MouseMoved, 0, NoButton, false, false, false, TouchScreen);
    if (m_currentContextNode->dispatchMouseEvent(mouseEvent, eventNames().contextmenuEvent, 0)) {
        context.setFlag(Platform::WebContext::IsOnContextMenuPrevented);
        return context;
    }

    requestLayoutIfNeeded();

    bool nodeAllowSelectionOverride = false;
    if (Node* linkNode = node->enclosingLinkEventParentOrSelf()) {
        KURL href;
        if (linkNode->isLink() && linkNode->hasAttributes()) {
            if (Attribute* attribute = static_cast<Element*>(linkNode)->getAttributeItem(HTMLNames::hrefAttr))
                href = linkNode->document()->completeURL(stripLeadingAndTrailingHTMLSpaces(attribute->value()));
        }

        String pattern = findPatternStringForUrl(href);
        if (!pattern.isEmpty())
            context.setPattern(pattern);

        if (!href.string().isEmpty()) {
            context.setUrl(href.string());

            if (linkNode->parentNode() && linkNode->parentNode()->canStartSelection())
                nodeAllowSelectionOverride = true;
        }
    }

    if (node->isHTMLElement()) {
        HTMLImageElement* imageElement = 0;
        HTMLMediaElement* mediaElement = 0;

        if (node->hasTagName(HTMLNames::imgTag))
            imageElement = static_cast<HTMLImageElement*>(node.get());
        else if (node->hasTagName(HTMLNames::areaTag))
            imageElement = static_cast<HTMLAreaElement*>(node.get())->imageElement();

        if (static_cast<HTMLElement*>(node.get())->isMediaElement())
            mediaElement = static_cast<HTMLMediaElement*>(node.get());

        if (imageElement && imageElement->renderer()) {
            context.setFlag(Platform::WebContext::IsImage);
            if (CachedResource* cachedResource = imageElement->cachedImage()) {
                if (cachedResource->isLoaded() && cachedResource->data()) {
                    String url = stripLeadingAndTrailingHTMLSpaces(imageElement->getAttribute(HTMLNames::srcAttr).string());
                    context.setSrc(node->document()->completeURL(url).string());
                }
            }
            String alt = imageElement->altText();
            if (!alt.isNull())
                context.setAlt(alt);
        }

        if (mediaElement) {
            if (mediaElement->hasAudio())
                context.setFlag(Platform::WebContext::IsAudio);
            if (mediaElement->hasVideo())
                context.setFlag(Platform::WebContext::IsVideo);

            String src = stripLeadingAndTrailingHTMLSpaces(mediaElement->getAttribute(HTMLNames::srcAttr).string());
            context.setSrc(node->document()->completeURL(src).string());
        }
    }

    if (node->isTextNode()) {
        Text* curText = toText(node.get());
        if (!curText->wholeText().isEmpty())
            context.setText(curText->wholeText());
    }

    bool canStartSelection = node->canStartSelection();

    if (node->isElementNode()) {
        Element* element = static_cast<Element*>(node->deprecatedShadowAncestorNode());

        if (DOMSupport::isTextBasedContentEditableElement(element)) {
            if (!canStartSelection) {
                Node* nodeUnderFinger = m_touchEventHandler->lastFatFingersResult().isValid() ? m_touchEventHandler->lastFatFingersResult().node(FatFingersResult::ShadowContentAllowed) : 0;
                if (nodeUnderFinger)
                    canStartSelection = nodeUnderFinger->canStartSelection();
            }
            context.setFlag(Platform::WebContext::IsInput);
            if (element->hasTagName(HTMLNames::inputTag))
                context.setFlag(Platform::WebContext::IsSingleLine);
            if (DOMSupport::isPasswordElement(element))
                context.setFlag(Platform::WebContext::IsPassword);

            String elementText(DOMSupport::inputElementText(element));
            if (!elementText.stripWhiteSpace().isEmpty())
                context.setText(elementText);
            else if (!node->focused() && m_touchEventHandler->lastFatFingersResult().isValid() && strategy == RectBased) {
                m_touchEventHandler->sendClickAtFatFingersPoint();
            }
        }
    }

    if (!nodeAllowSelectionOverride && !canStartSelection)
        context.resetFlag(Platform::WebContext::IsSelectable);

    if (node->isFocusable())
        context.setFlag(Platform::WebContext::IsFocusable);

    while (node) {
        if (node->isElementNode()) {
            Element* element = static_cast<Element*>(node->deprecatedShadowAncestorNode());
            String webWorksContext(DOMSupport::webWorksContext(element));
            if (!webWorksContext.stripWhiteSpace().isEmpty()) {
                context.setFlag(Platform::WebContext::IsWebWorksContext);
                context.setWebWorksContext(webWorksContext);
                break;
            }
        }
        node = node->parentNode();
    }

    return context;
}
