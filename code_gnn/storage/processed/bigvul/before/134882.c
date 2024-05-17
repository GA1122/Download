String WebPageSerializerImpl::postActionAfterSerializeOpenTag(
    const Element* element, SerializeDomParam* param)
{
    StringBuilder result;

    param->haveAddedContentsBeforeEnd = false;
    if (!param->isHTMLDocument)
        return result.toString();
    if (!param->haveAddedCharsetDeclaration
        && isHTMLHeadElement(*element)) {
        param->haveAddedCharsetDeclaration = true;
        result.append(WebPageSerializer::generateMetaCharsetDeclaration(
            String(param->textEncoding.name())));

        param->haveAddedContentsBeforeEnd = true;
    } else if (isHTMLScriptElement(*element) || isHTMLScriptElement(*element)) {
        param->isInScriptOrStyleTag = true;
    }

    return result.toString();
}
