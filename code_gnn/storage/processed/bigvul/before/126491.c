TabStripGtk::DropInfo::DropInfo(int drop_index, bool drop_before,
                                bool point_down)
    : drop_index(drop_index),
      drop_before(drop_before),
      point_down(point_down) {
  CreateContainer();
  drop_arrow = GetDropArrowImage(point_down);
}
