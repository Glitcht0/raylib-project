


#include "Colisores.h"


Font Carregarfonte() {
    int codepoints[256 - 32];
    for (int i = 32; i < 256; i++)
        codepoints[i - 32] = i;

    return LoadFontEx( "resources/fonts/arial.ttf", 48, codepoints, 256 - 32 );
}




