SwapXResResourceSizeValue(xXResResourceSizeValue *rep)
{
    SwapXResResourceSizeSpec(&rep->size);
    swapl(&rep->numCrossReferences);
}
