void BlobDataItem::detachFromCurrentThread()
{
    data->detachFromCurrentThread();
    path = path.isolatedCopy();
    url = url.copy();
}
