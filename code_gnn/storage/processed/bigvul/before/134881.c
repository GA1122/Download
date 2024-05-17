String WebPageSerializerImpl::postActionAfterSerializeEndTag(
    const Element* element, SerializeDomParam* param)
{
    StringBuilder result;

    if (!param->isHTMLDocument)
        return result.toString();
    if (isHTMLBaseElement(*element)) {
        result.appendLiteral("-->");
        result.append(WebPageSerializer::generateBaseTagDeclaration(
            param->document->baseTarget()));
    }

    return result.toString();
}
