const EC_METHOD *EC_GROUP_method_of(const EC_GROUP *group)
{
    return group->meth;
}
