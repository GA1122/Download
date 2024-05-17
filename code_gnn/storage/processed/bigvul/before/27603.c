int note_to_freq(int note_num)
{

	 

	int note, octave, note_freq;
	static int notes[] =
	{
		261632, 277189, 293671, 311132, 329632, 349232,
		369998, 391998, 415306, 440000, 466162, 493880
	};

#define BASE_OCTAVE	5

	octave = note_num / 12;
	note = note_num % 12;

	note_freq = notes[note];

	if (octave < BASE_OCTAVE)
		note_freq >>= (BASE_OCTAVE - octave);
	else if (octave > BASE_OCTAVE)
		note_freq <<= (octave - BASE_OCTAVE);

	 

	return note_freq;
}
