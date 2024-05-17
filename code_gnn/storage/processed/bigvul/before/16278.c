void SafeSock::getStat(unsigned long &noMsgs,
			     unsigned long &noWhole,
			     unsigned long &noDeleted,
			     unsigned long &avgMsgSize,
                       unsigned long &szComplete,
			     unsigned long &szDeleted)
{
	noMsgs = _noMsgs;
	noWhole = _whole;
	noDeleted = _deleted;
	avgMsgSize = _outMsg.getAvgMsgSize();
	szComplete = _avgSwhole;
	szDeleted = _avgSdeleted;
}
