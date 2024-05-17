ar6000_deliver_frames_to_nw_stack(void *dev, void *osbuf)
{
    struct sk_buff *skb = (struct sk_buff *)osbuf;

    if(skb) {
        skb->dev = dev;
        if ((skb->dev->flags & IFF_UP) == IFF_UP) {
#ifdef CONFIG_PM 
            ar6000_check_wow_status((struct ar6_softc *)ar6k_priv(dev), skb, false);
#endif  
            skb->protocol = eth_type_trans(skb, skb->dev);
         
            if (in_interrupt()) {
                netif_rx(skb);
            } else {
                netif_rx_ni(skb);
            }
        } else {
            A_NETBUF_FREE(skb);
        }
    }
}
