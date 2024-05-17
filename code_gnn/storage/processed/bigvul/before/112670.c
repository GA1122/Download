bool DocumentLoader::isMultipartReplacingLoad() const
{
    return isLoadingMultipartContent() && frameLoader()->isReplacing();
}
