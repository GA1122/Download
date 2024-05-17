  static int get_registers(pegasus_t *pegasus, __u16 indx, __u16 size, void *data)
  {
// 	u8 *buf;
  	int ret;
  
// 	buf = kmalloc(size, GFP_NOIO);
// 	if (!buf)
// 		return -ENOMEM;
// 
  	ret = usb_control_msg(pegasus->usb, usb_rcvctrlpipe(pegasus->usb, 0),
  			      PEGASUS_REQ_GET_REGS, PEGASUS_REQT_READ, 0,
			      indx, data, size, 1000);
// 			      indx, buf, size, 1000);
  	if (ret < 0)
  		netif_dbg(pegasus, drv, pegasus->net,
  			  "%s returned %d\n", __func__, ret);
// 	else if (ret <= size)
// 		memcpy(data, buf, ret);
// 	kfree(buf);
  	return ret;
  }