#include "render/font/fconfig.h"
#include "render/font/font.h"

void fttest() {
  FT_Library lib;
  FT_Face face;
  FT_Init_FreeType(&lib);

  char *ffile = match_font_desc("IBM Plex Mono");
  log_info("%s", ffile);
  free(ffile);
}
