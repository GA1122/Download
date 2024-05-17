static int forward_bnep(tETH_HDR* eth_hdr, BT_HDR *hdr) {
 int broadcast = eth_hdr->h_dest[0] & 1;

 for (int i = 0; i < MAX_PAN_CONNS; i++)
 {
        UINT16 handle = btpan_cb.conns[i].handle;
 if (handle != (UINT16)-1 &&
 (broadcast || memcmp(btpan_cb.conns[i].eth_addr, eth_hdr->h_dest, sizeof(BD_ADDR)) == 0
 || memcmp(btpan_cb.conns[i].peer, eth_hdr->h_dest, sizeof(BD_ADDR)) == 0)) {
 int result = PAN_WriteBuf(handle, eth_hdr->h_dest, eth_hdr->h_src, ntohs(eth_hdr->h_proto), hdr, 0);
 switch (result) {
 case PAN_Q_SIZE_EXCEEDED:
 return FORWARD_CONGEST;
 case PAN_SUCCESS:
 return FORWARD_SUCCESS;
 default:
 return FORWARD_FAILURE;
 }
 }
 }
    GKI_freebuf(hdr);
 return FORWARD_IGNORE;
}
