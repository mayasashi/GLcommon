#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

int finish() {
	char str[128];
	printf("Press any key to continue.\n");
	scanf_s("%c", &str[0]);
	return 0;
}

int main(void)
{
	FT_Library library;
	if (FT_Init_FreeType(&library) != FT_Err_Ok) {
		printf("SUSPENDED (%s) : an error occurred during library initialization.\n",__func__);
		return finish();
	}

	FT_Face face;
	if (FT_New_Face(library, "C:\\Windows\\Fonts\\times.ttf", 0, &face) != FT_Err_Ok) {
		printf("SUSPENDED (%s) : an error occurred while loading a font face.\n", __func__);
		return finish();
	}
	if (FT_Set_Char_Size(face, 0, 10 * 64, 72, 72) != FT_Err_Ok) {
		printf("SUSPENDED (%s) : an error occurred while setting char size. \n", __func__);
		return finish();
	}

	FT_UInt glyph_index;
	glyph_index = FT_Get_Char_Index(face, '5');
	if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT) != FT_Err_Ok) {
		printf("WARNING (%s) : improper call of load glyph function. render result may be corrupted. \n", __func__);
	}
	if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL) != FT_Err_Ok) {
		printf("WARNING (%s) : improper call of render glyph function. render result may be corrupted. \n", __func__);
	}

	printf("rows : %d\n", face->glyph->bitmap.rows);
	printf("width : %d\n", face->glyph->bitmap.width);
	printf("pitch : %d\n", face->glyph->bitmap.pitch);
	printf("buffer : %x\n", face->glyph->bitmap.buffer);
	printf("pixel_mode : %d\n", face->glyph->bitmap.pixel_mode);
	if (face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY) {
		printf("num_greys : %d\n", face->glyph->bitmap.num_grays);
	}

	int w = face->glyph->bitmap.width;
	int h = face->glyph->bitmap.rows;
	unsigned char *b = face->glyph->bitmap.buffer;

	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			if (b[i + w*j] > 128) printf("Å°");
			else printf("Å†");
		}
		printf("\n");
	}

	
	return finish();
}