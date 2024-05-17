static bool shouldCheckForCycles(int depth)
{
    ASSERT(depth >= 0);
    return !(depth & (depth - 1));
}
