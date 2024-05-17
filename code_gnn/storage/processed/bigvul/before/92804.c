void PrintHelp()
{
	fprintf(stderr, "MP4Client command keys:\n"
	        "\tq: quit\n"
	        "\tX: kill\n"
	        "\to: connect to the specified URL\n"
	        "\tO: connect to the specified playlist\n"
	        "\tN: switch to the next URL in the playlist. Also works with \\n\n"
	        "\tP: jumps to a given number ahead in the playlist\n"
	        "\tr: reload current presentation\n"
	        "\tD: disconnects the current presentation\n"
	        "\tG: selects object or service ID\n"
	        "\n"
	        "\tp: play/pause the presentation\n"
	        "\ts: step one frame ahead\n"
	        "\tz: seek into presentation by percentage\n"
	        "\tT: seek into presentation by time\n"
	        "\tt: print current timing\n"
	        "\n"
	        "\tu: sends a command (BIFS or LASeR) to the main scene\n"
	        "\te: evaluates JavaScript code\n"
	        "\tZ: dumps output video to PNG\n"
	        "\n"
	        "\tw: view world info\n"
	        "\tv: view Object Descriptor list\n"
	        "\ti: view Object Descriptor info (by ID)\n"
	        "\tj: view Object Descriptor info (by number)\n"
	        "\tb: view media objects timing and buffering info\n"
	        "\tm: view media objects buffering and memory info\n"
	        "\td: dumps scene graph\n"
	        "\n"
	        "\tk: turns stress mode on/off\n"
	        "\tn: changes navigation mode\n"
	        "\tx: reset to last active viewpoint\n"
	        "\n"
	        "\t3: switch OpenGL on or off for 2D scenes\n"
	        "\n"
	        "\t4: forces 4/3 Aspect Ratio\n"
	        "\t5: forces 16/9 Aspect Ratio\n"
	        "\t6: forces no Aspect Ratio (always fill screen)\n"
	        "\t7: forces original Aspect Ratio (default)\n"
	        "\n"
	        "\tL: changes to new log level. CF MP4Client usage for possible values\n"
	        "\tT: select new tools to log. CF MP4Client usage for possible values\n"
	        "\n"
	        "\tl: list available modules\n"
	        "\tc: prints some GPAC configuration info\n"
	        "\tE: forces reload of GPAC configuration\n"
	        "\n"
	        "\tR: toggles run-time info display in window title bar on/off\n"
	        "\tF: toggle displaying of FPS in stderr on/off\n"
	        "\tg: print GPAC allocated memory\n"
	        "\th: print this message\n"
	        "\n"
	        "\tEXPERIMENTAL/UNSTABLE OPTIONS\n"
	        "\tC: Enable Streaming Cache\n"
	        "\tS: Stops Streaming Cache and save to file\n"
	        "\tA: Aborts Streaming Cache\n"
	        "\tM: specifies video cache memory for 2D objects\n"
	        "\n"
	        "MP4Client - GPAC command line player - version %s\n"
	        "GPAC Written by Jean Le Feuvre (c) 2001-2005 - ENST (c) 2005-200X\n",

	        GPAC_FULL_VERSION
	       );
}