void MSG_Clear( msg_t *buf ) {
	buf->cursize = 0;
	buf->overflowed = qfalse;
	buf->bit = 0;					 
}