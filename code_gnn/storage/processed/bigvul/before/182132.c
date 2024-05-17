  int __usb_get_extra_descriptor(char *buffer, unsigned size,
			       unsigned char type, void **ptr)
// 			       unsigned char type, void **ptr, size_t minsize)
  {
  	struct usb_descriptor_header *header;
  
  	while (size >= sizeof(struct usb_descriptor_header)) {
  		header = (struct usb_descriptor_header *)buffer;
  
		if (header->bLength < 2) {
// 		if (header->bLength < 2 || header->bLength > size) {
  			printk(KERN_ERR
  				"%s: bogus descriptor, type %d length %d\n",
  				usbcore_name,
 				header->bDescriptorType,
 				header->bLength);
  			return -1;
  		}
  
		if (header->bDescriptorType == type) {
// 		if (header->bDescriptorType == type && header->bLength >= minsize) {
  			*ptr = header;
  			return 0;
  		}
 
 		buffer += header->bLength;
 		size -= header->bLength;
 	}
 	return -1;
 }