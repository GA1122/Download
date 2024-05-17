 static void setup_format_params(int track)
 {
 	int n;
 	int il;
 	int count;
 	int head_shift;
 	int track_shift;
 	struct fparm {
 		unsigned char track, head, sect, size;
 	} *here = (struct fparm *)floppy_track_buffer;
 
 	raw_cmd = &default_raw_cmd;
 	raw_cmd->track = track;
 
 	raw_cmd->flags = (FD_RAW_WRITE | FD_RAW_INTR | FD_RAW_SPIN |
 			  FD_RAW_NEED_DISK | FD_RAW_NEED_SEEK);
 	raw_cmd->rate = _floppy->rate & 0x43;
 	raw_cmd->cmd_count = NR_F;
 	COMMAND = FM_MODE(_floppy, FD_FORMAT);
 	DR_SELECT = UNIT(current_drive) + PH_HEAD(_floppy, format_req.head);
 	F_SIZECODE = FD_SIZECODE(_floppy);
 	F_SECT_PER_TRACK = _floppy->sect << 2 >> F_SIZECODE;
 	F_GAP = _floppy->fmt_gap;
 	F_FILL = FD_FILL_BYTE;
 
  	raw_cmd->kernel_data = floppy_track_buffer;
  	raw_cmd->length = 4 * F_SECT_PER_TRACK;
  
// 	if (!F_SECT_PER_TRACK)
// 		return;
// 
  	 
  	head_shift = (F_SECT_PER_TRACK + 5) / 6;
  
 	 
 	track_shift = 2 * head_shift + 3;
 
 	 
 	n = (track_shift * format_req.track + head_shift * format_req.head)
 	    % F_SECT_PER_TRACK;
 
 	 
 	il = 1;
 	if (_floppy->fmt_gap < 0x22)
 		il++;
 
 	 
 	for (count = 0; count < F_SECT_PER_TRACK; ++count) {
 		here[count].track = format_req.track;
 		here[count].head = format_req.head;
 		here[count].sect = 0;
 		here[count].size = F_SIZECODE;
 	}
 	 
 	for (count = 1; count <= F_SECT_PER_TRACK; ++count) {
 		here[n].sect = count;
 		n = (n + il) % F_SECT_PER_TRACK;
 		if (here[n].sect) {	 
 			++n;
 			if (n >= F_SECT_PER_TRACK) {
 				n -= F_SECT_PER_TRACK;
 				while (here[n].sect)
 					++n;
 			}
 		}
 	}
 	if (_floppy->stretch & FD_SECTBASEMASK) {
 		for (count = 0; count < F_SECT_PER_TRACK; count++)
 			here[count].sect += FD_SECTBASE(_floppy) - 1;
 	}
 }