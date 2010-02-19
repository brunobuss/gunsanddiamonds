#include "gnd.h"

void CarregaBitmap (BITMAP ** bitmap, char nome[])
{
	/* TODO LEAK: Verificar se bitmap ja tem algo */
	*bitmap = load_bitmap(nome, NULL);
	if (*bitmap == NULL)
	{
		printf("Erro ao carregar arquivo '%s'.\n", nome);
		FinalizaAllegro();
	}
}

void CarregaSom (SAMPLE ** som, char nome[])
{
	/* TODO LEAK: Verificar se som ja tem algo */
	*som = load_sample(nome);
	if (*som == NULL)
	{
		printf("Erro ao carregar arquivo '%s'.\n", nome);
		FinalizaAllegro();
	}
}

int MouseEstaEmAlgumBotao(int largura_botao, int altura_botao)
{
	if((mouse_x < X_INICIAL_BOTOES) ||
		(mouse_x > X_INICIAL_BOTOES + largura_botao) ||
		(mouse_y < Y_INICIAL_BOTOES) ||
		(mouse_y >= Y_INICIAL_BOTOES + (altura_botao) * 6)) return 0;

	return 1;
}

void SeparaFrames(char nome[], BITMAP * bitmap[], int frames)
{
	int i;
	BITMAP * temp;
	CarregaBitmap(&temp, nome);
	for (i=0; i<frames ; i++)
	{
		/* TODO LEAK: Verificar se bitmap[i] ja tem algo */
		bitmap[i] = create_bitmap( temp->w/frames , temp->h );
		blit(temp, bitmap[i], i*bitmap[0]->w, 0, 0,0, bitmap[0]->w, bitmap[0]->h);
	}
	destroy_bitmap(temp);
}