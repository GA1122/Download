static unsigned long long toIntegerMilliseconds(double seconds)
{
    ASSERT(seconds >= 0);
    return static_cast<unsigned long long>(seconds * 1000.0);
}
