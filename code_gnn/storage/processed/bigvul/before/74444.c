VerifyUdpChecksum(
                tCompletePhysicalAddress *pDataPages,
                ULONG ulDataLength,
                ULONG ulStartOffset,
                tTcpIpPacketParsingResult known,
                ULONG whatToFix)
{
    USHORT  phcs;
    tTcpIpPacketParsingResult res = known;
    IPHeader *pIpHeader = (IPHeader *)RtlOffsetToPointer(pDataPages[0].Virtual, ulStartOffset);
    UDPHeader *pUdpHeader = (UDPHeader *)RtlOffsetToPointer(pIpHeader, res.ipHeaderSize);
    USHORT saved = pUdpHeader->udp_xsum;
    USHORT xxpHeaderAndPayloadLen = GetXxpHeaderAndPayloadLen(pIpHeader, res);
    if (ulDataLength >= res.ipHeaderSize)
    {
        phcs = CalculateIpPseudoHeaderChecksum(pIpHeader, res, xxpHeaderAndPayloadLen);
        res.xxpCheckSum = CompareNetCheckSumOnEndSystem(phcs, saved) ?  ppresPCSOK : ppresCSBad;
        if (whatToFix & pcrFixPHChecksum)
        {
            if (ulDataLength >= (ULONG)(res.ipHeaderSize + sizeof(UDPHeader)))
            {
                pUdpHeader->udp_xsum = phcs;
                res.fixedXxpCS = res.xxpCheckSum != ppresPCSOK;
            }
            else
                res.xxpStatus = ppresXxpIncomplete;
        }
        else if (res.xxpCheckSum != ppresPCSOK || (whatToFix & pcrFixXxpChecksum))
        {
            if (res.xxpFull)
            {
                pUdpHeader->udp_xsum = phcs;
                CalculateUdpChecksumGivenPseudoCS(pUdpHeader, pDataPages, ulStartOffset + res.ipHeaderSize, xxpHeaderAndPayloadLen);
                if (CompareNetCheckSumOnEndSystem(pUdpHeader->udp_xsum, saved))
                    res.xxpCheckSum = ppresCSOK;

                if (!(whatToFix & pcrFixXxpChecksum))
                    pUdpHeader->udp_xsum = saved;
                else
                    res.fixedXxpCS =
                        res.xxpCheckSum == ppresCSBad || res.xxpCheckSum == ppresPCSOK;
            }
            else
                res.xxpCheckSum = ppresXxpIncomplete;
        }
        else if (res.xxpFull)
        {
            CalculateUdpChecksumGivenPseudoCS(pUdpHeader, pDataPages, ulStartOffset + res.ipHeaderSize, xxpHeaderAndPayloadLen);
            if (CompareNetCheckSumOnEndSystem(pUdpHeader->udp_xsum, saved))
                res.xxpCheckSum = ppresCSOK;
            pUdpHeader->udp_xsum = saved;
        }
    }
    else
        res.ipCheckSum = ppresIPTooShort;

    return res;
}