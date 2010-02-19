#include "gnd.h"

int menu_ja_foi_aberto=0;

BITMAP * buffer;
BITMAP * fundomenu;


void IniciaJogo (void)
{
	BITMAP * mouse;
	FILE * arqopcoes;	
	
	buffer = create_bitmap (RESOLUCAO_X, RESOLUCAO_Y);

	CarregaBitmap(&mouse, "./imgs/mouse/mira.tga");
	set_mouse_sprite(mouse);
	set_mouse_sprite_focus((mouse->w)/2, (mouse->h)/2);

	/* TODO: Define com o nome do arquivo de opcoes */
	/* TODO: Funcoes proprias para leitura do arquivo de opcoes */
	arqopcoes = fopen("opcoes", "r");
	if (arqopcoes==NULL)
	{
		arqopcoes = fopen("opcoes", "w");
		volume = 128;
		fprintf(arqopcoes, "%d", volume);
		fclose(arqopcoes);
	}
	else
	{
		fscanf(arqopcoes, "%d", &volume);
		fclose(arqopcoes);
	}
	if (volume>255)
		volume = 255;
	if (volume<0)
		volume = 0;
	set_volume(volume,volume);
}

int Menu (void)
{
	int i;
	int linha, ultimo_desenhado = -1;
	int altura_botao, largura_botao;
	BITMAP * menu[6];
	SAMPLE * tiro;

	CarregaSom(&tiro, "./sound/tiro.wav");
	CarregaBitmap(&fundomenu, "./imgs/menu/menu.tga");
	SeparaFrames("./imgs/menu/botoesmenu.tga", menu, 6);
	/* TODO LEAK: Nao eh necessario recarregar os recursos todas as vezes */
	
	if (!menu_ja_foi_aberto)
	{
		EfeitoAbrir(fundomenu,0,0, 1,1,0,0, 2);
		menu_ja_foi_aberto=1;
	}
	else
	{
		show_mouse(NULL);
		draw_sprite(screen, fundomenu, 0,0);
		show_mouse(screen);
	}
	
	altura_botao = menu[0]->h;
	largura_botao = menu[0]->w;
	
	while (mouse_b & 1);
	
	while (!(mouse_b & 1) || (ultimo_desenhado == -1))
	{
		linha = (mouse_y - Y_INICIAL_BOTOES)/menu[0]->h;
		
		if(MouseEstaEmAlgumBotao(largura_botao, altura_botao) && (ultimo_desenhado != linha))
		{
			show_mouse(NULL);
			draw_sprite(screen, fundomenu, 0, 0);
			
			if(linha == 6) linha = 5; /* TODO Melhorar: Workaround para bug de truncamento de float -> inteiro... =/ */

			draw_sprite(screen, menu[linha], X_INICIAL_BOTOES, linha*menu[0]->h + Y_INICIAL_BOTOES);
			show_mouse(screen);
			ultimo_desenhado = linha;
		}
		else if(!MouseEstaEmAlgumBotao(largura_botao, altura_botao) && (ultimo_desenhado != -1))
		{
			show_mouse(NULL);
			draw_sprite(screen, fundomenu, 0, 0);
			show_mouse(screen);
			ultimo_desenhado = -1;
		}
	}
	
	play_sample(tiro, 255, 128, 1000, 0);
	
	for (i=0; i<6 ; i++) destroy_bitmap(menu[i]);
	/* TODO: Remover quando nao for mais recarregar os recursos todas as vezes */
	
	return linha;
}

/* IniciaNovoJogo */
void Jogo (void)
{
	/* TODO: Para musica do menu e inicia musica do jogo */
	NovoJogo();
	
	/* TODO: Para musica do jogo e inicia musica do menu */	
}

/* Show Records */
void Recordes (void)
{
	int estasobre=0;
	int rec_pontos[10];
	int i, j;
	int pontos[6];
	char nomes[10][21];
	BITMAP * recordes;
	BITMAP * botaomenu;
	BITMAP *letras[26];
	BITMAP *numeros[10];

	CarregaBitmap(&recordes, "./imgs/recordes.tga");
	CarregaBitmap(&botaomenu, "./imgs/botaomenu.tga");
	SeparaFrames("./imgs/frames/numeros.tga", numeros, 10);
	SeparaFrames("./imgs/frames/letras.tga", letras, 26);
	/* TODO: Não precisa recarregar as imagens todas as vezes */
	
	EfeitoAbrir(recordes,1, fundomenu, 1, 1, 0, 0, 20);
	
	for(i=0;i<10;i++) rec_pontos[i] = 0;

	for(i=0;i<10;i++)
		for(j=0;j<21;j++) nomes[i][j] = 0;


	CarregaRecordes(nomes, rec_pontos);

	for(i=0;(rec_pontos[i] != 0 && i<10);i++)
	{
		for(j=0;nomes[i][j]!= 0;j++)
		{
			draw_sprite(buffer, letras[nomes[i][j] - 'A'], 144 + 21 * j, 240 + 27 * i);
		}

		PegaNumeroNaCasa(rec_pontos[i], 6, pontos);

		/* TODO: Melhorar para (pontos[i]%9 - 1) */
		if(pontos[5] >= 1)
		{
			draw_sprite(buffer, numeros[pontos[5] - 1], MENU_RECORDES_P_X + 19 * 0, MENU_RECORDES_P_Y + 27 *i + 2);
		}
		else
		{
			draw_sprite(buffer, numeros[9], MENU_RECORDES_P_X + 19 * 0, MENU_RECORDES_P_Y + 27 *i + 2);
		}

		if(pontos[4] >= 1)
		{
			draw_sprite(buffer, numeros[pontos[4] - 1], MENU_RECORDES_P_X + 19 * 1, MENU_RECORDES_P_Y + 27 *i + 2);
		}
		else
		{
			draw_sprite(buffer, numeros[9], MENU_RECORDES_P_X + 19 * 1, MENU_RECORDES_P_Y + 27 *i + 2);
		}

		if(pontos[3] >= 1)
		{
			draw_sprite(buffer, numeros[pontos[3] - 1], MENU_RECORDES_P_X + 19 * 2, MENU_RECORDES_P_Y + 27 *i + 2);
		}
		else
		{
			draw_sprite(buffer, numeros[9], MENU_RECORDES_P_X + 19 * 2, MENU_RECORDES_P_Y + 27 *i + 2);
		}

		if(pontos[2] >= 1)
		{
			draw_sprite(buffer, numeros[pontos[2] - 1], MENU_RECORDES_P_X + 19 * 3, MENU_RECORDES_P_Y + 27 *i + 2);
		}
		else
		{
			draw_sprite(buffer, numeros[9], MENU_RECORDES_P_X + 19 * 3, MENU_RECORDES_P_Y + 27 *i + 2);
		}

		if(pontos[1] >= 1)
		{
			draw_sprite(buffer, numeros[pontos[1] - 1], MENU_RECORDES_P_X + 19 * 4, MENU_RECORDES_P_Y + 27 *i + 2);
		}
		else
		{
			draw_sprite(buffer, numeros[9], MENU_RECORDES_P_X + 19 * 4, MENU_RECORDES_P_Y + 27 *i + 2);
		}

		if(pontos[0] >= 1)
		{
			draw_sprite(buffer, numeros[pontos[0] - 1], MENU_RECORDES_P_X + 19 * 5, MENU_RECORDES_P_Y + 27 *i + 2);
		}
		else
		{
			draw_sprite(buffer, numeros[9], MENU_RECORDES_P_X + 19 * 5, MENU_RECORDES_P_Y + 27 *i + 2);
		}
	}

	show_mouse(NULL);
	blit(buffer, screen, 0,0,0,0, 800, 600);
	show_mouse(screen);

	while (!key[KEY_ESC])
	{
		if (mouse_x>=X_MENU && mouse_x<=X_MENU + botaomenu->w && mouse_y>=Y_MENU && mouse_y<=Y_MENU + botaomenu->h)
		{
			if (estasobre==0)
			{
				show_mouse(NULL);
				draw_sprite(screen, botaomenu,X_MENU, Y_MENU);
				show_mouse(screen);
				estasobre=1;
			}
			if (mouse_b & 1)
			{
				while (mouse_b & 1);
				break;
			}
		}
		
		/* TODO: Não redesenhar a tela inteira apenas para "apagar" o botão de volta ao menu */
		if ((mouse_x<X_MENU || mouse_x>X_MENU + botaomenu->w || mouse_y<Y_MENU || mouse_y>Y_MENU + botaomenu->h) && estasobre==1)
		{
		  
			estasobre=0;
			show_mouse(NULL);
			draw_sprite(buffer, recordes,0, 0);

			for(i=0;(rec_pontos[i] != 0 && i<10);i++)
			{
				for(j=0;nomes[i][j]!= 0;j++)
				{
					draw_sprite(buffer, letras[nomes[i][j] - 'A'], 144 + 21 * j, 240 + 27 * i);
				}

				PegaNumeroNaCasa(rec_pontos[i], 6, pontos);

				if(pontos[5] >= 1)
				{
					draw_sprite(buffer, numeros[pontos[5] - 1], MENU_RECORDES_P_X + 19 * 0, MENU_RECORDES_P_Y + 27 *i + 2);
				}
				else
				{
					draw_sprite(buffer, numeros[9], MENU_RECORDES_P_X + 19 * 0, MENU_RECORDES_P_Y + 27 *i + 2);
				}

				if(pontos[4] >= 1)
				{
					draw_sprite(buffer, numeros[pontos[4] - 1], MENU_RECORDES_P_X + 19 * 1, MENU_RECORDES_P_Y + 27 *i + 2);
				}
				else
				{
					draw_sprite(buffer, numeros[9], MENU_RECORDES_P_X + 19 * 1, MENU_RECORDES_P_Y + 27 *i + 2);
				}

				if(pontos[3] >= 1)
				{
					draw_sprite(buffer, numeros[pontos[3] - 1], MENU_RECORDES_P_X + 19 * 2, MENU_RECORDES_P_Y + 27 *i + 2);
				}
				else
				{
					draw_sprite(buffer, numeros[9], MENU_RECORDES_P_X + 19 * 2, MENU_RECORDES_P_Y + 27 *i + 2);
				}

				if(pontos[2] >= 1)
				{
					draw_sprite(buffer, numeros[pontos[2] - 1], MENU_RECORDES_P_X + 19 * 3, MENU_RECORDES_P_Y + 27 *i + 2);
				}
				else
				{
					draw_sprite(buffer, numeros[9], MENU_RECORDES_P_X + 19 * 3, MENU_RECORDES_P_Y + 27 *i + 2);
				}

				if(pontos[1] >= 1)
				{
					draw_sprite(buffer, numeros[pontos[1] - 1], MENU_RECORDES_P_X + 19 * 4, MENU_RECORDES_P_Y + 27 *i + 2);
				}
				else
				{
					draw_sprite(buffer, numeros[9], MENU_RECORDES_P_X + 19 * 4, MENU_RECORDES_P_Y + 27 *i + 2);
				}

				if(pontos[0] >= 1)
				{
					draw_sprite(buffer, numeros[pontos[0] - 1], MENU_RECORDES_P_X + 19 * 5, MENU_RECORDES_P_Y + 27 *i + 2);
				}
				else
				{
					draw_sprite(buffer, numeros[9], MENU_RECORDES_P_X + 19 * 5, MENU_RECORDES_P_Y + 27 *i + 2);
				}
			}
			draw_sprite(screen, buffer,0,0);
			show_mouse(screen);
		}
	}
	
	destroy_bitmap(recordes);
	destroy_bitmap(botaomenu);
	
	/* TODO: Não precisa recarregar as imagens todas as vezes */
	for (i=0; i<26 ;i++)
	{
		if (i<10)
			destroy_bitmap(numeros[i]);
		destroy_bitmap(letras[i]);
	}
}