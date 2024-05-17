NDIS_STATUS ParaNdis_InitializeContext(
    PARANDIS_ADAPTER *pContext,
    PNDIS_RESOURCE_LIST pResourceList)
{
    NDIS_STATUS status = NDIS_STATUS_SUCCESS;
    USHORT linkStatus = 0;
    UCHAR CurrentMAC[ETH_LENGTH_OF_ADDRESS] = {0};
    ULONG dependentOptions;

    DEBUG_ENTRY(0);

    ReadNicConfiguration(pContext, CurrentMAC);

    pContext->fCurrentLinkState = MediaConnectStateUnknown;
    pContext->powerState = NdisDeviceStateUnspecified;

    pContext->MaxPacketSize.nMaxFullSizeOS = pContext->MaxPacketSize.nMaxDataSize + ETH_HEADER_SIZE;
    pContext->MaxPacketSize.nMaxFullSizeHwTx = pContext->MaxPacketSize.nMaxFullSizeOS;
#if PARANDIS_SUPPORT_RSC
    pContext->MaxPacketSize.nMaxDataSizeHwRx = MAX_HW_RX_PACKET_SIZE;
    pContext->MaxPacketSize.nMaxFullSizeOsRx = MAX_OS_RX_PACKET_SIZE;
#else
    pContext->MaxPacketSize.nMaxDataSizeHwRx = pContext->MaxPacketSize.nMaxFullSizeOS + ETH_PRIORITY_HEADER_SIZE;
    pContext->MaxPacketSize.nMaxFullSizeOsRx = pContext->MaxPacketSize.nMaxFullSizeOS;
#endif
    if (pContext->ulPriorityVlanSetting)
        pContext->MaxPacketSize.nMaxFullSizeHwTx = pContext->MaxPacketSize.nMaxFullSizeOS + ETH_PRIORITY_HEADER_SIZE;

    if (GetAdapterResources(pResourceList, &pContext->AdapterResources) &&
        NDIS_STATUS_SUCCESS == NdisMRegisterIoPortRange(
            &pContext->pIoPortOffset,
            pContext->MiniportHandle,
            pContext->AdapterResources.ulIOAddress,
            pContext->AdapterResources.IOLength)
        )
    {
        if (pContext->AdapterResources.InterruptFlags & CM_RESOURCE_INTERRUPT_MESSAGE)
        {
            DPrintf(0, ("[%s] Message interrupt assigned\n", __FUNCTION__));
            pContext->bUsingMSIX = TRUE;
        }

        VirtIODeviceInitialize(pContext->IODevice, pContext->AdapterResources.ulIOAddress, sizeof(*pContext->IODevice));
        VirtIODeviceSetMSIXUsed(pContext->IODevice, pContext->bUsingMSIX ? true : false);
        ParaNdis_ResetVirtIONetDevice(pContext);
        VirtIODeviceAddStatus(pContext->IODevice, VIRTIO_CONFIG_S_ACKNOWLEDGE);
        VirtIODeviceAddStatus(pContext->IODevice, VIRTIO_CONFIG_S_DRIVER);
        pContext->u32HostFeatures = VirtIODeviceReadHostFeatures(pContext->IODevice);
        DumpVirtIOFeatures(pContext);

        pContext->bLinkDetectSupported = AckFeature(pContext, VIRTIO_NET_F_STATUS);
        if(pContext->bLinkDetectSupported) {
            VirtIODeviceGet(pContext->IODevice, ETH_LENGTH_OF_ADDRESS, &linkStatus, sizeof(linkStatus));
            pContext->bConnected = (linkStatus & VIRTIO_NET_S_LINK_UP) != 0;
            DPrintf(0, ("[%s] Link status on driver startup: %d\n", __FUNCTION__, pContext->bConnected));
        }

        InitializeMAC(pContext, CurrentMAC);

        pContext->bUseMergedBuffers = AckFeature(pContext, VIRTIO_NET_F_MRG_RXBUF);
        pContext->nVirtioHeaderSize = (pContext->bUseMergedBuffers) ? sizeof(virtio_net_hdr_ext) : sizeof(virtio_net_hdr_basic);
        pContext->bDoPublishIndices = AckFeature(pContext, VIRTIO_RING_F_EVENT_IDX);
    }
    else
    {
        DPrintf(0, ("[%s] Error: Incomplete resources\n", __FUNCTION__));
         
        pContext->AdapterResources.ulIOAddress = 0;
        status = NDIS_STATUS_RESOURCE_CONFLICT;
    }

    pContext->bMultiQueue = AckFeature(pContext, VIRTIO_NET_F_CTRL_MQ);
    if (pContext->bMultiQueue)
    {
        VirtIODeviceGet(pContext->IODevice, ETH_LENGTH_OF_ADDRESS + sizeof(USHORT), &pContext->nHardwareQueues,
            sizeof(pContext->nHardwareQueues));
    }
    else
    {
        pContext->nHardwareQueues = 1;
    }

    dependentOptions = osbT4TcpChecksum | osbT4UdpChecksum | osbT4TcpOptionsChecksum;

    if((pContext->Offload.flagsValue & dependentOptions) && !AckFeature(pContext, VIRTIO_NET_F_CSUM))
    {
        DPrintf(0, ("[%s] Host does not support CSUM, disabling CS offload\n", __FUNCTION__) );
        pContext->Offload.flagsValue &= ~dependentOptions;
    }

    pContext->bGuestChecksumSupported = AckFeature(pContext, VIRTIO_NET_F_GUEST_CSUM);
    AckFeature(pContext, VIRTIO_NET_F_CTRL_VQ);

    InitializeRSCState(pContext);

    ParaNdis_ResetOffloadSettings(pContext, NULL, NULL);

    if (pContext->Offload.flags.fTxLso && !AckFeature(pContext, VIRTIO_NET_F_HOST_TSO4))
    {
        DisableLSOv4Permanently(pContext, __FUNCTION__, "Host does not support TSOv4\n");
    }

    if (pContext->Offload.flags.fTxLsov6 && !AckFeature(pContext, VIRTIO_NET_F_HOST_TSO6))
    {
        DisableLSOv6Permanently(pContext, __FUNCTION__, "Host does not support TSOv6");
    }

    pContext->bUseIndirect = AckFeature(pContext, VIRTIO_F_INDIRECT);
    pContext->bAnyLaypout = AckFeature(pContext, VIRTIO_F_ANY_LAYOUT);

    pContext->bHasHardwareFilters = AckFeature(pContext, VIRTIO_NET_F_CTRL_RX_EXTRA);

    InterlockedExchange(&pContext->ReuseBufferRegular, TRUE);

    VirtIODeviceWriteGuestFeatures(pContext->IODevice, pContext->u32GuestFeatures);
    NdisInitializeEvent(&pContext->ResetEvent);
    DEBUG_EXIT_STATUS(0, status);
    return status;
}