#include "gnd.h"

int IniciaAllegro(void)
{
	allegro_init();

	if (install_keyboard())
	{
		puts("Programa nao iniciado por erro no teclado.");
		allegro_message("Last Allegro error `%s'\n", allegro_error);
		return 0;
	}

	if (install_mouse() == -1)
	{
		puts("Programa nao iniciado por erro no mouse.");
		allegro_message("Last Allegro error `%s'\n", allegro_error);
		return 0;
	}
	
	install_timer();

	if (install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL) < 0)
	{
		puts("Programa nao iniciado por erro no som.");
		allegro_message("Last Allegro error `%s'\n", allegro_error);
		return 0;
	}

	set_color_depth(COLOR_DEPTH);
	if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, RESOLUCAO_X, RESOLUCAO_Y, 0, 0)<0)
	{
		puts("Programa nao iniciado por erro no modo grafico.");
		allegro_message("Last Allegro error `%s'\n", allegro_error);
		return 0;
	}

	return 1;
}


void FinalizaAllegro (void)
{
	allegro_exit();
}