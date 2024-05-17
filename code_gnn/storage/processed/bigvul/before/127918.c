void DocumentThreadableLoader::setDefersLoading(bool value)
{
    if (resource())
        resource()->setDefersLoading(value);
}
