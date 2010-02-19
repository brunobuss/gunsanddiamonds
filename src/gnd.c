#include "gnd.h"

int main (void)
{
	int i = 1;
	
	srand(time(NULL));
	
	if (!IniciaAllegro()) return 1;
	
	IniciaJogo();
	Intro();

	while (i)
	{
		clear_keybuf();
		switch (Menu())
		{
			case 0:
				while (mouse_b&1);
				clear_keybuf();
				Jogo();
				break;
			case 1:
				while (mouse_b&1);
				clear_keybuf();
				Recordes();
				break;
			case 2:
				while (mouse_b&1);
				clear_keybuf();
				Opcoes();
				break;
			case 3:
				while (mouse_b&1);
				clear_keybuf();
				Ajuda();
				break;
			case 4:
				while (mouse_b&1);
				clear_keybuf();
				Creditos();
				break;
			case 5:
				while (mouse_b&1);
				clear_keybuf();
				Sair();
				i=0;
				break;
		}
	}


	FinalizaAllegro();
	return 0;
}
END_OF_MAIN()