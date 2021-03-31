#include "render/font/fconfig.h"
#include "render/font/font.h"

#include "render/opengl.h"

unsigned int fttest() {
  int err;
  FT_Library lib;
  FT_Face face;
  FT_Init_FreeType(&lib);
  char *ffile = match_font_desc("DejaVu Sans Mono");
  if (!ffile)
    return -1;
  log_info("%s", ffile);
  err = FT_New_Face(lib, ffile, 0, &face);
  if (err) {
    log_error("err load");
  }
  log_info("face color: %d", FT_HAS_COLOR(face));
  free(ffile);

  err = FT_Set_Char_Size(face,    // handle to face object
                         0,       // char_width in 1/64th of points
                         16 * 64, // char_height in 1/64th of points
                         300,     // horizontal device resolution
                         300);    // vertical device resolution
  if (err) {
    log_error("err size");
  }

  log_info("global metrics:\n max advance: %d\n max height: %d",
           face->size->metrics.max_advance >> 6,
           face->size->metrics.height >> 6);
  log_info("number of glyphs: %d", face->num_glyphs);

  FT_UInt gindex = FT_Get_Char_Index(face, 'A');
  err = FT_Load_Glyph(face, gindex, FT_LOAD_DEFAULT | FT_LOAD_COLOR);
  if (err) {
    log_error("err glyph");
  }
  err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LCD);
  if (err) {
    log_error("err render");
  }

  FT_GlyphSlot glyph = face->glyph;

  log_info("glyph metrics:\n advance: %d\n height: %d",
           glyph->metrics.horiAdvance >> 6, face->size->metrics.height >> 6);

  log_info("bitmap loaded: %d, %d", face->glyph->bitmap.rows,
           face->glyph->bitmap.width);
  log_info("color bitmap?: %d", glyph->bitmap.pixel_mode);

  unsigned int texture;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  log_info("loaded image");
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, glyph->bitmap.width / 3,
               glyph->bitmap.rows, 0, GL_RGB, GL_UNSIGNED_BYTE,
               glyph->bitmap.buffer);

  glGenerateMipmap(GL_TEXTURE_2D);
  return texture;
}
