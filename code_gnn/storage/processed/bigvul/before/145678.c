void ImageInputType::altAttributeChanged()
{
    if (element().userAgentShadowRoot()) {
        Element* text = element().userAgentShadowRoot()->getElementById("alttext");
        String value = element().altText();
        if (text && text->textContent() != value)
            text->setTextContent(element().altText());
    }
}
