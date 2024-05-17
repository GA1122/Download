void ImageInputType::reattachFallbackContent()
{
    if (element().document().inStyleRecalc())
        element().reattach();
    else
        element().lazyReattachIfAttached();
}
