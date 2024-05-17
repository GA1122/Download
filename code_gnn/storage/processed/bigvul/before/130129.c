void HTMLBodyElement::parseAttribute(const QualifiedName& name, const AtomicString& value)
{
    if (name == vlinkAttr || name == alinkAttr || name == linkAttr) {
        if (value.isNull()) {
            if (name == linkAttr)
                document().textLinkColors().resetLinkColor();
            else if (name == vlinkAttr)
                document().textLinkColors().resetVisitedLinkColor();
            else
                document().textLinkColors().resetActiveLinkColor();
        } else {
            RGBA32 color;
            if (BisonCSSParser::parseColor(color, value, !document().inQuirksMode())) {
                if (name == linkAttr)
                    document().textLinkColors().setLinkColor(color);
                else if (name == vlinkAttr)
                    document().textLinkColors().setVisitedLinkColor(color);
                else
                    document().textLinkColors().setActiveLinkColor(color);
            }
        }

        setNeedsStyleRecalc(SubtreeStyleChange);
    } else if (name == onloadAttr)
        document().setWindowAttributeEventListener(EventTypeNames::load, createAttributeEventListener(document().frame(), name, value));
    else if (name == onbeforeunloadAttr)
        document().setWindowAttributeEventListener(EventTypeNames::beforeunload, createAttributeEventListener(document().frame(), name, value));
    else if (name == onunloadAttr)
        document().setWindowAttributeEventListener(EventTypeNames::unload, createAttributeEventListener(document().frame(), name, value));
    else if (name == onpagehideAttr)
        document().setWindowAttributeEventListener(EventTypeNames::pagehide, createAttributeEventListener(document().frame(), name, value));
    else if (name == onpageshowAttr)
        document().setWindowAttributeEventListener(EventTypeNames::pageshow, createAttributeEventListener(document().frame(), name, value));
    else if (name == onpopstateAttr)
        document().setWindowAttributeEventListener(EventTypeNames::popstate, createAttributeEventListener(document().frame(), name, value));
    else if (name == onblurAttr)
        document().setWindowAttributeEventListener(EventTypeNames::blur, createAttributeEventListener(document().frame(), name, value));
    else if (name == onerrorAttr)
        document().setWindowAttributeEventListener(EventTypeNames::error, createAttributeEventListener(document().frame(), name, value));
    else if (name == onfocusAttr)
        document().setWindowAttributeEventListener(EventTypeNames::focus, createAttributeEventListener(document().frame(), name, value));
    else if (RuntimeEnabledFeatures::orientationEventEnabled() && name == onorientationchangeAttr)
        document().setWindowAttributeEventListener(EventTypeNames::orientationchange, createAttributeEventListener(document().frame(), name, value));
    else if (name == onhashchangeAttr)
        document().setWindowAttributeEventListener(EventTypeNames::hashchange, createAttributeEventListener(document().frame(), name, value));
    else if (name == onmessageAttr)
        document().setWindowAttributeEventListener(EventTypeNames::message, createAttributeEventListener(document().frame(), name, value));
    else if (name == onresizeAttr)
        document().setWindowAttributeEventListener(EventTypeNames::resize, createAttributeEventListener(document().frame(), name, value));
    else if (name == onscrollAttr)
        document().setWindowAttributeEventListener(EventTypeNames::scroll, createAttributeEventListener(document().frame(), name, value));
    else if (name == onselectionchangeAttr)
        document().setAttributeEventListener(EventTypeNames::selectionchange, createAttributeEventListener(document().frame(), name, value));
    else if (name == onstorageAttr)
        document().setWindowAttributeEventListener(EventTypeNames::storage, createAttributeEventListener(document().frame(), name, value));
    else if (name == ononlineAttr)
        document().setWindowAttributeEventListener(EventTypeNames::online, createAttributeEventListener(document().frame(), name, value));
    else if (name == onofflineAttr)
        document().setWindowAttributeEventListener(EventTypeNames::offline, createAttributeEventListener(document().frame(), name, value));
    else
        HTMLElement::parseAttribute(name, value);
}
