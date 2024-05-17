Packet *PacketGetFromQueueOrAlloc(void)
{
     
    Packet *p = PacketPoolGetPacket();

    if (p == NULL) {
         
        p = PacketGetFromAlloc();
    } else {
        PACKET_PROFILING_START(p);
    }

    return p;
}
