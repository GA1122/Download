int saa7164_bus_set(struct saa7164_dev *dev, struct tmComResInfo* msg,
	void *buf)
{
	struct tmComResBusInfo *bus = &dev->bus;
	u32 bytes_to_write, free_write_space, timeout, curr_srp, curr_swp;
	u32 new_swp, space_rem;
	int ret = SAA_ERR_BAD_PARAMETER;
	u16 size;

	if (!msg) {
		printk(KERN_ERR "%s() !msg\n", __func__);
		return SAA_ERR_BAD_PARAMETER;
	}

	dprintk(DBGLVL_BUS, "%s()\n", __func__);

	saa7164_bus_verify(dev);

	if (msg->size > dev->bus.m_wMaxReqSize) {
		printk(KERN_ERR "%s() Exceeded dev->bus.m_wMaxReqSize\n",
			__func__);
		return SAA_ERR_BAD_PARAMETER;
	}

	if ((msg->size > 0) && (buf == NULL)) {
		printk(KERN_ERR "%s() Missing message buffer\n", __func__);
		return SAA_ERR_BAD_PARAMETER;
	}

	 
	mutex_lock(&bus->lock);

	bytes_to_write = sizeof(*msg) + msg->size;
	free_write_space = 0;
	timeout = SAA_BUS_TIMEOUT;
	curr_srp = saa7164_readl(bus->m_dwSetReadPos);
	curr_swp = saa7164_readl(bus->m_dwSetWritePos);

	 
	if (curr_srp > curr_swp)
		 
		free_write_space = curr_srp - curr_swp;
	else
		 
		free_write_space = (curr_srp + bus->m_dwSizeSetRing) - curr_swp;

	dprintk(DBGLVL_BUS, "%s() bytes_to_write = %d\n", __func__,
		bytes_to_write);

	dprintk(DBGLVL_BUS, "%s() free_write_space = %d\n", __func__,
		free_write_space);

	dprintk(DBGLVL_BUS, "%s() curr_srp = %x\n", __func__, curr_srp);
	dprintk(DBGLVL_BUS, "%s() curr_swp = %x\n", __func__, curr_swp);

	 
	while (bytes_to_write >= free_write_space) {

		if (timeout-- == 0) {
			printk(KERN_ERR "%s() bus timeout\n", __func__);
			ret = SAA_ERR_NO_RESOURCES;
			goto out;
		}

		 
		 
		mdelay(1);

		 
		curr_srp = saa7164_readl(bus->m_dwSetReadPos);

		 
		if (curr_srp > curr_swp)
			 
			free_write_space = curr_srp - curr_swp;
		else
			 
			free_write_space = (curr_srp + bus->m_dwSizeSetRing) -
				curr_swp;

	}

	 
	new_swp = curr_swp + bytes_to_write;

	dprintk(DBGLVL_BUS, "%s() new_swp = %x\n", __func__, new_swp);
	dprintk(DBGLVL_BUS, "%s() bus->m_dwSizeSetRing = %x\n", __func__,
		bus->m_dwSizeSetRing);

	 
	size = msg->size;
	 
	msg->size = (__force u16)cpu_to_le16(msg->size);
	msg->command = (__force u32)cpu_to_le32(msg->command);
	msg->controlselector = (__force u16)cpu_to_le16(msg->controlselector);

	 

	 
	if (new_swp > bus->m_dwSizeSetRing) {

		 
		new_swp -= bus->m_dwSizeSetRing;

		space_rem = bus->m_dwSizeSetRing - curr_swp;

		dprintk(DBGLVL_BUS, "%s() space_rem = %x\n", __func__,
			space_rem);

		dprintk(DBGLVL_BUS, "%s() sizeof(*msg) = %d\n", __func__,
			(u32)sizeof(*msg));

		if (space_rem < sizeof(*msg)) {
			dprintk(DBGLVL_BUS, "%s() tr4\n", __func__);

			 
			memcpy_toio(bus->m_pdwSetRing + curr_swp, msg, space_rem);
			memcpy_toio(bus->m_pdwSetRing, (u8 *)msg + space_rem,
				sizeof(*msg) - space_rem);

			memcpy_toio(bus->m_pdwSetRing + sizeof(*msg) - space_rem,
				buf, size);

		} else if (space_rem == sizeof(*msg)) {
			dprintk(DBGLVL_BUS, "%s() tr5\n", __func__);

			 
			memcpy_toio(bus->m_pdwSetRing + curr_swp, msg, sizeof(*msg));
			memcpy_toio(bus->m_pdwSetRing, buf, size);

		} else {
			 
			memcpy_toio(bus->m_pdwSetRing + curr_swp, msg, sizeof(*msg));
			if (size > 0) {
				memcpy_toio(bus->m_pdwSetRing + curr_swp +
					sizeof(*msg), buf, space_rem -
					sizeof(*msg));
				memcpy_toio(bus->m_pdwSetRing, (u8 *)buf +
					space_rem - sizeof(*msg),
					bytes_to_write - space_rem);
			}

		}

	}  
	else {
		dprintk(DBGLVL_BUS, "%s() tr6\n", __func__);

		 
		memcpy_toio(bus->m_pdwSetRing + curr_swp, msg, sizeof(*msg));
		memcpy_toio(bus->m_pdwSetRing + curr_swp + sizeof(*msg), buf,
			size);
	}

	dprintk(DBGLVL_BUS, "%s() new_swp = %x\n", __func__, new_swp);

	 
	saa7164_writel(bus->m_dwSetWritePos, new_swp);

	 
	msg->size = le16_to_cpu((__force __le16)msg->size);
	msg->command = le32_to_cpu((__force __le32)msg->command);
	msg->controlselector = le16_to_cpu((__force __le16)msg->controlselector);
	ret = SAA_OK;

out:
	saa7164_bus_dump(dev);
	mutex_unlock(&bus->lock);
	saa7164_bus_verify(dev);
	return ret;
}