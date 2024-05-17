cell_command_is_var_length(uint8_t command, int linkproto)
{
   
  switch (linkproto) {
  case 1:
     
    return 0;
  case 2:
     
    return command == CELL_VERSIONS;
  case 0:
  case 3:
  default:
     
    return command == CELL_VERSIONS || command >= 128;
  }
}
