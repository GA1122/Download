CompareNetCheckSumOnEndSystem(USHORT computedChecksum, USHORT arrivedChecksum)
{
    if(arrivedChecksum == 0xFFFF)
        arrivedChecksum = 0;

    return computedChecksum == arrivedChecksum;
}
