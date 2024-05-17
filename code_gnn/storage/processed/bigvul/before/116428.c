static bool isIdentity(const WebTransformOperation* operation)
{
    return !operation || operation->isIdentity();
}
