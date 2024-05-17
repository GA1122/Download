static int usage() {
    ERROR("usage: sdcard [OPTIONS] <source_path> <label>\n"
 "    -u: specify UID to run as\n"
 "    -g: specify GID to run as\n"
 "    -U: specify user ID that owns device\n"
 "    -m: source_path is multi-user\n"
 "    -w: runtime write mount has full write access\n"
 "\n");
 return 1;
}
