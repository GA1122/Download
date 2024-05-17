  int flush_completed_IO(struct inode *inode)
  {
  	ext4_io_end_t *io;
// 	struct ext4_inode_info *ei = EXT4_I(inode);
// 	unsigned long flags;
  	int ret = 0;
  	int ret2 = 0;
  
	if (list_empty(&EXT4_I(inode)->i_completed_io_list))
// 	if (list_empty(&ei->i_completed_io_list))
  		return ret;
  
  	dump_completed_IO(inode);
	while (!list_empty(&EXT4_I(inode)->i_completed_io_list)){
		io = list_entry(EXT4_I(inode)->i_completed_io_list.next,
// 	spin_lock_irqsave(&ei->i_completed_io_lock, flags);
// 	while (!list_empty(&ei->i_completed_io_list)){
// 		io = list_entry(ei->i_completed_io_list.next,
  				ext4_io_end_t, list);
  		 
// 		spin_unlock_irqrestore(&ei->i_completed_io_lock, flags);
  		ret = ext4_end_io_nolock(io);
// 		spin_lock_irqsave(&ei->i_completed_io_lock, flags);
  		if (ret < 0)
  			ret2 = ret;
  		else
  			list_del_init(&io->list);
  	}
// 	spin_unlock_irqrestore(&ei->i_completed_io_lock, flags);
  	return (ret2 < 0) ? ret2 : 0;
  }