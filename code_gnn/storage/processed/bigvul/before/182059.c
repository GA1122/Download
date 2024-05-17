 tChecksumCheckResult ParaNdis_CheckRxChecksum(
                                             PARANDIS_ADAPTER *pContext,
                                             ULONG virtioFlags,
                                             tCompletePhysicalAddress *pPacketPages,
                                             ULONG ulPacketLength,
                                              ULONG ulDataOffset)
  {
      tOffloadSettingsFlags f = pContext->Offload.flags;
    tChecksumCheckResult res, resIp;
//     tChecksumCheckResult res;
      tTcpIpPacketParsingResult ppr;
      ULONG flagsToCalculate = 0;
      res.value = 0;
    resIp.value = 0;
  
 
     if (f.fRxIPChecksum) flagsToCalculate |= pcrIpChecksum;  
 
     if (!(virtioFlags & VIRTIO_NET_HDR_F_DATA_VALID))
     {
         if (virtioFlags & VIRTIO_NET_HDR_F_NEEDS_CSUM)
         {
             flagsToCalculate |= pcrFixXxpChecksum | pcrTcpChecksum | pcrUdpChecksum;
         }
         else
         {
             if (f.fRxTCPChecksum) flagsToCalculate |= pcrTcpV4Checksum;
             if (f.fRxUDPChecksum) flagsToCalculate |= pcrUdpV4Checksum;
             if (f.fRxTCPv6Checksum) flagsToCalculate |= pcrTcpV6Checksum;
             if (f.fRxUDPv6Checksum) flagsToCalculate |= pcrUdpV6Checksum;
         }
     }
  
      ppr = ParaNdis_CheckSumVerify(pPacketPages, ulPacketLength - ETH_HEADER_SIZE, ulDataOffset + ETH_HEADER_SIZE, flagsToCalculate, __FUNCTION__);
  
//     if (ppr.ipCheckSum == ppresIPTooShort || ppr.xxpStatus == ppresXxpIncomplete)
//     {
//         res.flags.IpOK = FALSE;
//         res.flags.IpFailed = TRUE;
//         return res;
//     }
// 
      if (virtioFlags & VIRTIO_NET_HDR_F_DATA_VALID)
      {
          pContext->extraStatistics.framesRxCSHwOK++;
         ppr.xxpCheckSum = ppresCSOK;
     }
 
     if (ppr.ipStatus == ppresIPV4 && !ppr.IsFragment)
     {
         if (f.fRxIPChecksum)
         {
             res.flags.IpOK =  ppr.ipCheckSum == ppresCSOK;
             res.flags.IpFailed = ppr.ipCheckSum == ppresCSBad;
         }
         if(ppr.xxpStatus == ppresXxpKnown)
         {
             if(ppr.TcpUdp == ppresIsTCP)  
             {
                 if (f.fRxTCPChecksum)
                 {
                     res.flags.TcpOK = ppr.xxpCheckSum == ppresCSOK || ppr.fixedXxpCS;
                     res.flags.TcpFailed = !res.flags.TcpOK;
                 }
             }
             else  
             {
                 if (f.fRxUDPChecksum)
                 {
                     res.flags.UdpOK = ppr.xxpCheckSum == ppresCSOK || ppr.fixedXxpCS;
                     res.flags.UdpFailed = !res.flags.UdpOK;
                 }
             }
         }
     }
     else if (ppr.ipStatus == ppresIPV6)
     {
         if(ppr.xxpStatus == ppresXxpKnown)
         {
             if(ppr.TcpUdp == ppresIsTCP)  
             {
                 if (f.fRxTCPv6Checksum)
                 {
                     res.flags.TcpOK = ppr.xxpCheckSum == ppresCSOK || ppr.fixedXxpCS;
                     res.flags.TcpFailed = !res.flags.TcpOK;
                 }
             }
             else  
             {
                 if (f.fRxUDPv6Checksum)
                 {
                     res.flags.UdpOK = ppr.xxpCheckSum == ppresCSOK || ppr.fixedXxpCS;
                     res.flags.UdpFailed = !res.flags.UdpOK;
                 }
             }
         }
     }
 
     return res;
 }