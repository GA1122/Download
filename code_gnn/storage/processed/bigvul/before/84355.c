	void ConvertToMPT(ModSample &mptSmp) const
	{
		mptSmp.nLength = length;
		mptSmp.nVolume = 4u * std::min<uint16>(volume, 64);

		mptSmp.nLoopStart = loopStart;
		mptSmp.nLoopEnd = loopStart + loopLength;

		if(mptSmp.nLoopStart >= mptSmp.nLength)
		{
			mptSmp.nLoopStart = mptSmp.nLength - 1;
		}
		if(mptSmp.nLoopEnd > mptSmp.nLength)
		{
			mptSmp.nLoopEnd = mptSmp.nLength;
		}

		if(mptSmp.nLoopStart > mptSmp.nLoopEnd)
		{
			mptSmp.nLoopStart = 0;
			mptSmp.nLoopEnd = 0;
		} else if(mptSmp.nLoopEnd > mptSmp.nLoopStart)
		{
			mptSmp.uFlags.set(CHN_LOOP);
			mptSmp.cues[0] = mptSmp.nLoopStart;
		}
	}
