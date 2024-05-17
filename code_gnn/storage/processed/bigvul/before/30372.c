static bool vmci_transport_dgram_allow(u32 cid, u32 port)
{
	if (cid == VMADDR_CID_HYPERVISOR) {
		 
		return port == VMCI_UNITY_PBRPC_REGISTER;
	}

	return true;
}
