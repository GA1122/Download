_XRRHasOutputPrimary (int major, int minor)
{
    return major > 1 || (major == 1 && minor >= 3);
}
