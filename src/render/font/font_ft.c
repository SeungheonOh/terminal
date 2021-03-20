#include "render/font/fconfig.h"
#include "render/font/font.h"

#include "render/opengl.h"

#define STB_IMAGE_IMPLEMENTATION
#include "render/font/stb_image.h"

unsigned int fttest() {
  int err;
  FT_Library lib;
  FT_Face face;
  FT_Init_FreeType(&lib);
  char *ffile = match_font_desc("IBM Plex");
  err = FT_New_Face(lib, ffile, 0, &face);
  if (err) {
    log_error("err load");
  }
  log_info("%s", ffile);
  log_info("face color: %d", FT_HAS_COLOR(face));
  free(ffile);

  /*
  FT_Set_Char_Size(face,    // handle to face object
                   0,       // char_width in 1/64th of points
                   16 * 64, // char_height in 1/64th of points
                   300,     // horizontal device resolution
                   300);
                   */

  err = FT_Set_Pixel_Sizes(face, // handle to face object
                           0,    // pixel_width
                           48);  // pixel_height
  if (err) {
    log_error("err size");
  }

  FT_UInt gindex = FT_Get_Char_Index(face, '5');
  err = FT_Load_Glyph(face, gindex, FT_LOAD_DEFAULT | FT_LOAD_COLOR);
  if (err) {
    log_error("err glyph");
  }
  err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LCD);
  if (err) {
    log_error("err render");
  }

  FT_GlyphSlot glyph = face->glyph;

  log_info("bitmap loaded: %d, %d", face->glyph->bitmap.rows,
           face->glyph->bitmap.width);
  log_info("color bitmap?: %d", glyph->bitmap.pixel_mode == FT_PIXEL_MODE_LCD);
  /*
  for (int i = 0; i < glyph->bitmap.rows; i++) {
    for (int j = 0; j < glyph->bitmap.width; j++) {
      if (glyph->bitmap.buffer[j + i * glyph->bitmap.width])
        printf("*");
      else
        printf(" ");
    }
    printf("\n");
  }
  */

  unsigned int texture;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  log_info("loaded image");
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
  //             GL_UNSIGNED_BYTE, data);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph->bitmap.width,
               glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
               glyph->bitmap.buffer);

  glGenerateMipmap(GL_TEXTURE_2D);
  return texture;
}
