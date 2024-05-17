void P2PSocketDispatcherHost::DoGetNetworkList(int routing_id) {
  net::NetworkInterfaceList list;
  net::GetNetworkList(&list);
  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE, NewRunnableMethod(
          this, &P2PSocketDispatcherHost::SendNetworkList, routing_id, list));
}
