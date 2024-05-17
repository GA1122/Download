static bool isOriginPotentiallyTrustworthy(SecurityOrigin* origin, String* errorMessage)
{
    if (errorMessage)
        return origin->isPotentiallyTrustworthy(*errorMessage);
    return origin->isPotentiallyTrustworthy();
}
