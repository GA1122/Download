static double getPossiblySkewedTimeInKnownRange(double skewedTime, double lowerBound, double upperBound)
{
#if PLATFORM(CHROMIUM)
    if (skewedTime <= lowerBound)
        return lowerBound;

    if (skewedTime >= upperBound)
        return upperBound;
#else
    ASSERT_UNUSED(lowerBound, skewedTime >= lowerBound);
    ASSERT_UNUSED(upperBound, skewedTime <= upperBound);
#endif

    return skewedTime;
}
