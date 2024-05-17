double WebPagePrivate::clampedScale(double scale) const
{
    if (scale < minimumScale())
        return minimumScale();
    if (scale > maximumScale())
        return maximumScale();
    return scale;
}
