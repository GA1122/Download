 vhost_scsi_send_evt(struct vhost_scsi *vs,
 		   struct vhost_scsi_tpg *tpg,
 		   struct se_lun *lun,
 		   u32 event,
 		   u32 reason)
 {
 	struct vhost_scsi_evt *evt;
 
 	evt = vhost_scsi_allocate_evt(vs, event, reason);
 	if (!evt)
 		return;
 
 	if (tpg && lun) {
 		 
 		 
  		evt->event.lun[0] = 0x01;
		evt->event.lun[1] = tpg->tport_tpgt & 0xFF;
// 		evt->event.lun[1] = tpg->tport_tpgt;
  		if (lun->unpacked_lun >= 256)
  			evt->event.lun[2] = lun->unpacked_lun >> 8 | 0x40 ;
  		evt->event.lun[3] = lun->unpacked_lun & 0xFF;
 	}
 
 	llist_add(&evt->list, &vs->vs_event_list);
 	vhost_work_queue(&vs->dev, &vs->vs_event_work);
 }