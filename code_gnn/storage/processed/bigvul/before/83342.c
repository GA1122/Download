void CSoundFile::UpdateS3MEffectMemory(ModChannel *pChn, ModCommand::PARAM param) const
{
	pChn->nOldVolumeSlide = param;	 
	pChn->nOldPortaUp = param;		 
	pChn->nOldPortaDown = param;	 
	pChn->nTremorParam = param;		 
	pChn->nArpeggio = param;		 
	pChn->nRetrigParam = param;		 
	pChn->nTremoloDepth = (param & 0x0F) << 2;	 
	pChn->nTremoloSpeed = (param >> 4) & 0x0F;	 
}
