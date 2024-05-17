ResourceError DocumentLoader::interruptedForPolicyChangeError() const
{
    return frameLoader()->client()->interruptedForPolicyChangeError(request());
}
