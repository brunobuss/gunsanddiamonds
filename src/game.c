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
		if (i<10) destroy_bitmap(numeros[i]);
		destroy_bitmap(letras[i]);
	}
}


void Opcoes (void)
{
	int i;
	int estasobre=0;
	FILE * arqopcoes;
	BITMAP * opcoes;
	BITMAP * botaomenu;
	BITMAP * botaovolume;
	BITMAP * explode[20];
	SAMPLE * explosao;

	/* TODO: Não precisa recarregar as imagens todas as vezes */
	CarregaBitmap(&opcoes, "./imgs/opcoes/opcoes.tga");
	CarregaBitmap(&botaomenu, "./imgs/botaomenu.tga");
	CarregaBitmap(&botaovolume, "./imgs/opcoes/botaovolume.tga");
	SeparaFrames("./imgs/frames/explode.tga", explode, 20);
	CarregaSom (&explosao, "./sound/explosao.wav");
	
	EfeitoAbrir(opcoes,1, fundomenu, 1, 1, 0, 0, 20);
	clear_keybuf();
	show_mouse(NULL);
	draw_sprite(screen, botaovolume, X_INICIAL_VOLUME+volume*(X_FINAL_VOLUME-X_INICIAL_VOLUME)/255 - botaovolume->w/2, Y_VOLUME);
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
		if ((mouse_x<X_MENU || mouse_x>X_MENU + botaomenu->w || mouse_y<Y_MENU || mouse_y>Y_MENU + botaomenu->h) && estasobre==1)
		{
			estasobre=0;
			show_mouse(NULL);
			draw_sprite(buffer, opcoes,0, 0);
			draw_sprite(buffer, botaovolume, X_INICIAL_VOLUME+volume*(X_FINAL_VOLUME-X_INICIAL_VOLUME)/255 - botaovolume->w/2, Y_VOLUME);
			draw_sprite(screen, buffer,0,0);
			show_mouse(screen);
		}
		if (mouse_y>=Y_VOLUME && mouse_y<=Y_VOLUME + botaovolume->h && mouse_x>=X_INICIAL_VOLUME && mouse_x<X_FINAL_VOLUME && mouse_b&1)
		{
			while (mouse_b & 1)
			{
				if (mouse_x>X_INICIAL_VOLUME  && mouse_x <X_FINAL_VOLUME)
					volume=(mouse_x-X_INICIAL_VOLUME)*2;
				else
				{
					if (mouse_x<=X_INICIAL_VOLUME)
						volume=1;
					if (mouse_x>=X_FINAL_VOLUME)
						volume=255;
				}
				draw_sprite(buffer,opcoes,0,0);
				show_mouse(buffer);
				draw_sprite(buffer, botaovolume, X_INICIAL_VOLUME+volume*(X_FINAL_VOLUME-X_INICIAL_VOLUME)/255 - botaovolume->w/2 , Y_VOLUME);
				draw_sprite(screen, buffer,0,0);
			}
			stop_sample(explosao);
			set_volume(volume,volume);
			play_sample(explosao, 255, 128, 1000, 0);
			for (i=0; i<20 && !(mouse_b&1) && !keypressed() ; i++)
			{
				draw_sprite(buffer,opcoes,0,0);
				draw_sprite(buffer, explode[i], X_INICIAL_VOLUME+volume*(X_FINAL_VOLUME-X_INICIAL_VOLUME)/255 - explode[0]->w/2 , Y_VOLUME-60 );
				show_mouse(NULL);
				draw_sprite(screen,buffer,0,0);
				show_mouse(screen);
				rest(20);
			}
			draw_sprite(buffer,opcoes,0,0);
			draw_sprite(buffer, botaovolume, X_INICIAL_VOLUME+volume*(X_FINAL_VOLUME-X_INICIAL_VOLUME)/255 - botaovolume->w/2 , Y_VOLUME);
			show_mouse(NULL);
			draw_sprite(screen,buffer,0,0);
			show_mouse(screen);
		}
	}
	
	/* TODO: Não precisa recarregar as imagens todas as vezes */
	arqopcoes = fopen("opcoes", "w");
	fprintf(arqopcoes, "%d", volume);
	fclose(arqopcoes);
	destroy_bitmap(opcoes);
	destroy_bitmap(botaovolume);
	destroy_bitmap(botaomenu);
	for (i=0; i<20 ; i++)
		destroy_bitmap(explode[i]);
	destroy_sample(explosao);
}


void Ajuda (void)
{
	int estasobre=0;
	BITMAP * ajuda;
	BITMAP * botaomenu;
	
	/* TODO: Não precisa recarregar as imagens todas as vezes */
	CarregaBitmap(&ajuda, "./imgs/ajuda.tga");
	CarregaBitmap(&botaomenu, "./imgs/botaomenu.tga");

	EfeitoAbrir(ajuda,1, fundomenu, 1,1,0,0,20);
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
		if ((mouse_x<X_MENU || mouse_x>X_MENU + botaomenu->w || mouse_y<Y_MENU || mouse_y>Y_MENU + botaomenu->h) && estasobre==1)
		{
			estasobre=0;
			show_mouse(NULL);
			draw_sprite(buffer, ajuda,0, 0);
			draw_sprite(screen, buffer,0,0);
			show_mouse(screen);
		}
	}
	
	/* TODO: Não precisa recarregar as imagens todas as vezes */
	destroy_bitmap(ajuda);
	destroy_bitmap(botaomenu);
}


void Creditos (void)
{
	int i;

	BITMAP * atira[4];
	BITMAP * creditos;
	BITMAP * explode[20];
	BITMAP * granada;
	BITMAP * mendigo[12];
	BITMAP * recarrega[19];
	BITMAP * rocketeiro[11];
	BITMAP * tanque[12];

	SAMPLE * aplausos;
	SAMPLE * explosao;
	SAMPLE * recarrega1;
	SAMPLE * tiro;

	for (i=0; i<20 ; i++)
	{
		if (i==0)
		{
			granada=NULL;
			aplausos=NULL;
			explosao=NULL;
			recarrega1=NULL;
			tiro=NULL;
		}
		if (i<4)
			atira[i]=NULL;
		if (i<11)
			rocketeiro[i]=NULL;
		if (i<12)
		{
			mendigo[i]=NULL;
			tanque[i]=NULL;
		}
		if (i<19)
			recarrega[i]=NULL;
		explode[i]=NULL;
	}

	CarregaBitmap(&creditos, "./imgs/creditos/creditos.tga");

	clear_keybuf();

	for (i=0; i<1048 && !keypressed() && !(mouse_b & 1) ; i++)
	{
		show_mouse(NULL);
		clear_to_color(buffer,0);
		blit(creditos, buffer, 0,0, (RESOLUCAO_X - creditos->w)/2,RESOLUCAO_Y-i,creditos->w, i );

		if (i==Y_TACOWBERRY)
			SeparaFrames("./imgs/frames/atira.tga", atira, 4);
		if (i>=Y_TACOWBERRY && i<Y_FINAL_TACOWBERRY)
		{
			if (i==Y_RECARREGA)
				SeparaFrames("./imgs/frames/recarrega.tga", recarrega, 19);
			if (i>=Y_RECARREGA && i<Y_FINAL_RECARREGA)
			{
				draw_sprite(buffer,recarrega[(i/4)%19], X_CREDITOS, RESOLUCAO_Y - i + Y_TACOWBERRY);
				if (i==Y_RECARREGA+48)
				{
					CarregaSom (&recarrega1, "./sound/recarrega.wav");
					play_sample(recarrega1, 255, 128, 1000, 0);
				}
			}

			else
			{
				if (i==Y_FINAL_RECARREGA)
				{
					destroy_sample(recarrega1);
					recarrega1=NULL;
				}
					
				if (i>=Y_FINAL_RECARREGA && i<Y_FINAL_RECARREGA+19)
				{
					destroy_bitmap(recarrega[i-Y_FINAL_RECARREGA]);
					recarrega[i-Y_FINAL_RECARREGA]=NULL;
				}
				draw_sprite(buffer,atira[(i/4)%4], X_CREDITOS, RESOLUCAO_Y - i + Y_TACOWBERRY);
				if ((i/4)%4 == 0)
				{
					CarregaSom (&tiro, "./sound/tiro.wav");
					stop_sample(tiro);
					play_sample(tiro, 255/2, 128, 900, 0);
				}
			}
		}
		if (i==Y_FINAL_TACOWBERRY)
		{
			destroy_sample(tiro);
			tiro=NULL;
		}
		if (i>=Y_FINAL_TACOWBERRY && i<Y_FINAL_TACOWBERRY + 4)
		{
			destroy_bitmap(atira[i-Y_FINAL_TACOWBERRY]);
			atira[i-Y_FINAL_TACOWBERRY]=NULL;
		}

		/* Granada */
		if (i==Y_GRANADA)
			CarregaBitmap(&granada, "./imgs/frames/granada.tga");
		if (i>=Y_GRANADA && i<Y_FINAL_GRANADA)
			rotate_sprite(buffer, granada, X_CREDITOS+(i-Y_GRANADA)/2, RESOLUCAO_Y - i + Y_GRANADA, itofix(i*4));
		if (i==Y_FINAL_GRANADA)
		{
			destroy_bitmap(granada);
			granada=NULL;
		}
		if (i==Y_FINAL_GRANADA)
			SeparaFrames("./imgs/frames/explode.tga", explode, 20);
		if (i>=Y_FINAL_GRANADA && i<Y_FINAL_GRANADA+40)
		{
			draw_sprite(buffer,explode[(i-Y_FINAL_GRANADA)/2], X_CREDITOS + 183, RESOLUCAO_Y - i + Y_GRANADA - (explode[0]->h));
			if (i==Y_FINAL_GRANADA)
			{
				CarregaSom(&explosao, "./sound/explosao.wav");
				play_sample(explosao, 255, 128, 1000, 0);
			}
 		}
		/* Fim granada */

		if (i==Y_ROCKETEIRO)
			SeparaFrames("./imgs/frames/rocketeiro.tga", rocketeiro, 11);
		if (i>= Y_ROCKETEIRO && i<Y_ROCKETEIRO+300)
			draw_sprite(buffer, rocketeiro[0], X_CREDITOS-20, RESOLUCAO_Y - i + Y_ROCKETEIRO);
		if (i>=Y_ROCKETEIRO+300 && i<608)
			draw_sprite(buffer, rocketeiro[((i-Y_ROCKETEIRO-300)/3)], X_CREDITOS-20, RESOLUCAO_Y - i + Y_ROCKETEIRO);
		if (i>=608 && i<619)
		{
			destroy_bitmap(rocketeiro[i-608]);
			rocketeiro[i-608]=NULL;
		}
		if (i>=608 && i<648)
			draw_sprite(buffer,explode[(i-608)/2], X_CREDITOS, RESOLUCAO_Y - i +Y_ROCKETEIRO+25 - (explode[0]->h));
		if (i==648)
		{
			destroy_sample(explosao);
			explosao=NULL;
		}
		if (i>=648 && i<668)
		{
			destroy_bitmap(explode[i-648]);
			explode[i-648]=NULL;
		}

		if (i==Y_TANQUE)
			SeparaFrames("./imgs/frames/tanque.tga", tanque, 12);
		if (i>= Y_TANQUE && i<Y_FINAL_TANQUE)
			rotate_sprite_v_flip(buffer, tanque[(i/3)%5 +7], X_CREDITOS - 10, RESOLUCAO_Y - i + Y_TANQUE , itofix(128));
		if (i>=Y_FINAL_TANQUE && i<Y_FINAL_TANQUE+12)
		{
			destroy_bitmap(tanque[i-Y_FINAL_TANQUE]);
			tanque[i-Y_FINAL_TANQUE]=NULL;
		}

		if (i==Y_MENDIGO)
			SeparaFrames("./imgs/frames/mendigoanda.tga", mendigo, 12);
		if (i>=Y_MENDIGO && i<Y_FINAL_MENDIGO)
		{
			draw_sprite(buffer,mendigo[(i/3)%12], X_CREDITOS, RESOLUCAO_Y - i + Y_MENDIGO);
		}
		if (i>=Y_FINAL_MENDIGO && i<Y_FINAL_MENDIGO+12)
		{
			destroy_bitmap(tanque[i-Y_FINAL_TANQUE]);
			tanque[i-Y_FINAL_TANQUE]=NULL;
		}
		
		if (i==900)
		{
			CarregaSom (&aplausos, "./sound/aplausos.wav");
			play_sample(aplausos, 255, 128,1000,0);
		}
		if (i!=1047)
			show_mouse(buffer);

		draw_sprite(screen, buffer, 0, 0);
		rest(10);
	}
	show_mouse(screen);
	while (!keypressed() && !(mouse_b & 1));
	while (mouse_b & 1);
	stop_sample(aplausos);
	for (i=0; i<20 ; i++)
	{
		if (i==0)
		{
			destroy_bitmap(granada);
			destroy_sample(aplausos);
			destroy_sample(explosao);
			destroy_sample(recarrega1);
			destroy_sample(tiro);
		}
		if (i<4)
			destroy_bitmap(atira[i]);
		if (i<11)
			destroy_bitmap(rocketeiro[i]);
		if (i<12)
		{
			destroy_bitmap(mendigo[i]);
			destroy_bitmap(tanque[i]);
		}
		if (i<19)
			destroy_bitmap(recarrega[i]);
		destroy_bitmap(explode[i]);
	}
}


void Sair (void)
{
	BITMAP * logotipo;
	BITMAP * sair;
	sair = create_bitmap(RESOLUCAO_X, RESOLUCAO_Y);
	CarregaBitmap(&logotipo, "./imgs/logotipo.tga");
	clear_to_color(sair, 0);
	draw_sprite(sair, logotipo,13,0);
	textprintf_centre_ex(sair, font, RESOLUCAO_X/2, 500, makecol(255,255,255),0,  "Agradece por usar nossos programas");
	EfeitoAbrir(sair, 0,0, 1, 0,0,0, 3);
	rest(1500);
}


void Intro (void)
{
	int i, j=230, k=144;

	BITMAP * abaixa[4];
	BITMAP * anda[9];
	BITMAP * apresenta;
	BITMAP * atira[4];
	BITMAP * bomba;
	BITMAP * explode[20];
	BITMAP * explode2[20];
	BITMAP * fundo;
	BITMAP * granada;
	BITMAP * logotipo;
	BITMAP * mendigo[8];
	BITMAP * mendigoanda[12];
	BITMAP * missel;
	BITMAP * parado[3];
	BITMAP * pula[7];
	BITMAP * queima[20];
	BITMAP * recarrega[19];
	BITMAP * rocket[3];
	BITMAP * tacabomba[5];
	BITMAP * tanque[12];
	BITMAP * vira[2];

	SAMPLE * explosao;
	SAMPLE * grito;
	SAMPLE * missile;
	SAMPLE * passos;
	SAMPLE * recarrega1;
	SAMPLE * tank;
	SAMPLE * tiro;

	TocaMp3(&musicaintro, "./sound/musicaintro.mp3");
	install_int_ex(MusicaIntro, 10000);


	CarregaSom (&passos, "./sound/passos.wav");

	SeparaFrames("./imgs/frames/parado.tga", parado, 3);
	CarregaSom (&missile, "./sound/missile.wav");
	SeparaFrames("./imgs/frames/atira.tga", atira, 4);
	CarregaSom (&tiro, "./sound/tiro.wav");
	SeparaFrames("./imgs/frames/pula.tga", pula, 7);
	SeparaFrames("./imgs/frames/tanque.tga", tanque, 12);
	CarregaSom (&tank, "./sound/tank.wav");
	SeparaFrames("./imgs/frames/explode.tga", explode, 20);
	CarregaBitmap(&granada, "./imgs/frames/granada.tga");
	SeparaFrames("./imgs/frames/vira.tga", vira, 2);
	SeparaFrames("./imgs/frames/tacabomba.tga", tacabomba, 5);
	SeparaFrames("./imgs/frames/mendigo.tga", mendigo, 8);
	SeparaFrames("./imgs/frames/anda.tga", anda, 9);
	CarregaBitmap(&logotipo, "./imgs/logotipo.tga");
	CarregaBitmap(&fundo, "./imgs/muro.tga");
	CarregaBitmap(&bomba, "./imgs/frames/bomba.tga");
	SeparaFrames("./imgs/frames/recarrega.tga", recarrega, 19);
	CarregaSom (&recarrega1, "./sound/recarrega.wav");
	SeparaFrames("./imgs/frames/explode2.tga", explode2, 20);
	SeparaFrames("./imgs/frames/rocket.tga",rocket, 3);
	CarregaBitmap(&missel, "./imgs/frames/missel.tga");
	SeparaFrames("./imgs/frames/abaixa.tga", abaixa,4);
	CarregaSom (&grito, "./sound/grito.wav");
	SeparaFrames("./imgs/frames/queima.tga", queima, 20);
	CarregaBitmap(&apresenta, "./imgs/apresenta.tga");
	SeparaFrames("./imgs/frames/mendigoanda.tga", mendigoanda, 12);
	CarregaSom (&explosao, "./sound/explosao.wav");
	FadeIn(fundo, 15);

	for (i=0; i<730 && !keypressed() && !(mouse_b & 1) ;i++)
	{
		clear(buffer);
		draw_sprite(buffer, logotipo,0,0);
		draw_sprite(buffer, fundo,0,0);

		if (i<200) /* Mendigo correndo pra cima do Tacow Berry */
			rotate_sprite_v_flip(buffer, mendigo[i%8], 800-i*8, 200, itofix(128));

		if (i==1)
			play_sample(passos, 255, 128,1000,1);

		if (i<40) /* Tacow Berry correndo pra tacar a bomba */
			draw_sprite(buffer,anda[i%9], i*6, 200);

		if (i>=40 && i<45) /* Tacow Berry tacando bomba */
		{
			draw_sprite(buffer,tacabomba[(i-40)%5], 240, 200);
			stop_sample(passos);
		}

		if (i>=45 && i<47) /* Tacow Berry virando */
			draw_sprite(buffer, vira[i-45],240, 200);

		if (i>=40 && j<319) /* Bomba girando e caindo */
		{
			rotate_sprite(buffer, granada, j+20, (j-319)*(j-319)*0.002 + 180, itofix(j*3));
			j+=5;
		}

		if (i>=57 && i<77) /* Bomba explodindo */
		{
			draw_sprite(buffer, explode[i-57],j-(explode[0]->w)/2+20,(j-319)*(j-319)*0.002 + 188 - explode[0]->h);
			if (i==57)
				play_sample(explosao, 255, 128, 1000, 0);
		}

		if (i==58) /* Parede explodindo */
			Explosao (fundo, 1, 280, 118);

		if (i>=47 && i<97) /* Tacow Berry saindo para pegar tanque */
		{
			rotate_sprite_v_flip(buffer, anda[(i-47)%9], 240-(i-47)*6, 200, itofix(128));
			if (i==47)
				play_sample(passos, 255, 128,1000,1);
		}
		if (i==92)
			stop_sample(passos);

		if (i>=124 && i<283) /* Mendigo fugindo do tanque */
			draw_sprite(buffer, mendigo[(i-124)%8], (i-140)*6, 200);

		if (i>=160 && i<=228) /* Tanque andando at� engui�ar */
		{
			draw_sprite(buffer, tanque[(i-160)%12], (i-170)*6, 180);
			if (i == 160 || i == 185)
				play_sample(tank, 255/2, 128, 1000, 0);
			if (i==228)
				stop_sample(tank);
		}

		if (i>190 && i<207) /* Bomba do tanque*/
		{
			rotate_sprite(buffer, bomba, (i-190)*20+215, 200-(i-190)*2, itofix(i*4));
			if (i==206)
				play_sample(explosao, 255, 128, 1000, 0);
		}

		if (i>=207 && i<227) /* Bomba do tanque explodindo */
		{
			j = 538;
			draw_sprite(buffer, explode[i-207],j-(explode[0]->w)/2,(j-319)*(j-319)*0.002 + 121 - explode[0]->h);
			Explosao (fundo, 2, 430, 100);
		}

		if (i==238) /* Tacow Berry saindo do tanque */
			draw_sprite(buffer, pula[0],398, 170-(i-238)*7);

		if (i==239) /* Tacow Berry saindo do tanque */
			draw_sprite(buffer, pula[0],398, 170-(i-238)*5);

		if (i==240) /* Tacow Berry saindo do tanque */
			draw_sprite(buffer, pula[0],398, 170-(i-238));

		if (i>=228 && i<356) /* Tanque engui�ado */
			draw_sprite(buffer, tanque[10], 348, 180);

		if (i>=241 && i<247) /* Tacow Berry saindo do tanque */
			draw_sprite(buffer, pula[1],398, 170+(i-238)*4 -6);

		if (i>=247 && i<252) /* Tacow Berry saindo do tanque */
			draw_sprite(buffer, pula[i-245],398, 197);

		if (i==252)
			play_sample(passos, 255, 128, 1000, 1);

		if (i>=252 && i<300) /* Tacow Berry andando pra explodir o tanque */
			draw_sprite(buffer,anda[i%9], 398+(i-252)*6, 200);

		if (i==300)
			stop_sample(passos);

		if (i>=300 && i<303) /* Tacow Berry virando pra atirar no tanque */
			draw_sprite(buffer, vira[(i-300)/2], 690, 200);

		if (i>=303 && i<341) /* Tacow Berry recarregando */
		{
			rotate_sprite_v_flip(buffer, recarrega[(i-303)/2], 685, 200, itofix(128));
			if (i==335)
				play_sample(recarrega1, 255, 128, 1000, 0);
		}

		if (i>=341 && i<354) /* Tacow Berry atirando */
		{
			rotate_sprite_v_flip(buffer, atira[(i-341)%4], 676, 201, itofix(128));
			if (i==341 || i==347 || i==353)
				play_sample(tiro, 255, 128, 1000, 0);
		}

		if (i>=354 && i<374) /* Tanque explodindo */
		{
			draw_sprite(buffer, explode2[i-354], 388, 140);
			if (i==354)
				play_sample(explosao, 255, 128, 1000, 0);
			Explosao (fundo, 2.5, 290, 90);
			rectfill(fundo, 433, 177, 513, 237, makecol(255,0,255));
			rectfill(fundo, 489, 141, 517, 168, makecol(255,0,255));
			rectfill (fundo, 338, 194, 377, 248, makecol(255,0,255));
		}
			
		if (i>=308 && i<365) /* Rocketeiro subindo */
			draw_sprite(buffer,rocket[0], 300, RESOLUCAO_Y-i+308);

		if (i>=365 && i<409) /* Rocketeiro lan�ando missel */
		{
			rotate_sprite(buffer, missel, 320+(i-308-rocket[0]->h)*10, 532-(i-308-rocket[0]->h)*10, itofix(-32));
			draw_sprite(buffer,rocket[0], 300, RESOLUCAO_Y-rocket[0]->h);
			if (i==365)
				play_sample(missile, 255/2, 128, 1000, 0);
		}

		if (i>=354 && i<395) /* Tacow Berry descansando */
			rotate_sprite_v_flip(buffer, parado[((i-353)/6)%3], 695, 198, itofix(128));
		
		if (i>=395 && i<411) /* Tacow Berry abaixando */
			rotate_sprite_v_flip(buffer, abaixa[(i-395)/4], 695, 198, itofix(128));
		
		if (i>=409 && i<429) /* Missel explodindo */
		{
			draw_sprite(buffer, explode2[i-409], 690, 70);
			Explosao(fundo, 1.6, 640,70);
			if (i==409)
				play_sample(explosao, 255, 128, 1000, 0);
		}

		if (i>=411 && i<416) /* Tacow Berry abaixado */
			rotate_sprite_v_flip(buffer, abaixa[3], 695, 198, itofix(128));

		if (i>=416 && i<428) /* Tacow Berry levantando */
			rotate_sprite_v_flip(buffer, abaixa[(431-i)/4], 695, 198, itofix(128));

		if (i>=428 && i<549) /* Tacow Berry correndo */
		{
			rotate_sprite_v_flip(buffer, anda[(i-428)%9], 690-(i-428)*6, 198, itofix(128));
			if (i==428)
				play_sample(passos, 255, 128, 1000, 1);
		}

		if (i>=409 && i<443) /* Rocketeiro lancando segundo missel */
		{
			rotate_sprite(buffer, missel, 320+(i-409)*9, 532-(i-409)*10, itofix(-38));
			draw_sprite(buffer,rocket[0], 300, RESOLUCAO_Y-rocket[0]->h);
			if (i==409)
				play_sample(missile, 255/2, 128, 1000, 0);
		}

		if (i>=443 && i<463) /* Missel 2 explodindo */
		{
			draw_sprite(buffer, explode2[i-443], 601, 122);
			Explosao(fundo, 1.6, 541,120);
			rectfill(fundo, 573, 168, 638, 220, makecol(255,0,255));
			rectfill(fundo, 632, 113, 727, 201, makecol(255,0,255));
			rectfill(fundo, 673, 193, 721, 224, makecol(255,0,255));
			rectfill(fundo, 577, 148, 629, 179, makecol(255,0,255));
			if (i==443)
				play_sample(explosao, 255, 128, 1000, 0);
		}

		if (i>=443 && i<489) /* Rocketeiro lan�ando terceiro missel */
		{
			rotate_sprite(buffer, missel, 300+(i-443)*3.5, 532-(i-443)*10, itofix(-48));
			draw_sprite(buffer,rocket[1], 300, RESOLUCAO_Y-rocket[1]->h);
			if (i==443)
				play_sample(missile, 255/2, 128, 1000, 0);
		}

		if (i>=476 && i<510) /* Rocketeiro lancando quarto missel */
		{
			if (i<480)
				draw_sprite(buffer,rocket[1], 300, RESOLUCAO_Y-rocket[1]->h);
			rotate_sprite(buffer, missel, 300+(i-476)*3.3, 532-(i-476)*10, itofix(-50));
			if (i==476)
				play_sample(missile, 255/2, 128, 1000, 0);
		}

		if (i>=489 && i<509) /* Terceiro missel explodindo */
		{
			draw_sprite(buffer, explode2[i-489], 410, 0);
			draw_sprite(buffer, explode2[i-489], 430, 0);
			draw_sprite(buffer, explode2[i-489], 440, 0);
			Explosao(fundo, 4, 271,-100);
			rectfill(fundo, 573, 168, 638, 220, makecol(255,0,255));
			rectfill(fundo, 539, 146, 623, 279, makecol(255,0,255));
			rectfill(fundo, 500, 0, 556, 12, makecol(255,0,255));
			if (i==489)
				play_sample(explosao, 255, 128, 1000, 0);
		}

		if (i>=509 && i<529) /* Quarto missel explodindo */
		{
			draw_sprite(buffer, explode2[i-509], 410, 100);
			Explosao(fundo, 2, 271,50);
			rectfill(fundo, 371, 70, 424, 111, makecol(255,0,255));
			if (i==509)
				play_sample(explosao, 255, 128, 1000, 0);
		}

		if (i>=489 && i<521) /* Rocketeiro lancando 5 missel */
		{
			if (i<492)
				draw_sprite(buffer,rocket[2], 300, RESOLUCAO_Y-rocket[1]->h);
			rotate_sprite(buffer, missel, 290, 532-(i-489)*10, itofix(-64));
			if (i==489)
				play_sample(missile, 255/2, 128, 1000, 0);
		}

		if (i>=521 && i<541) /* Missel 5 explodindo */
		{
			draw_sprite(buffer, explode2[i-521], 270, 120);
			Explosao(fundo, 4, 111,00);
			rectfill(fundo, 384, 280, 429, 303, makecol(255,0,255));
			rectfill(fundo, 304, 59, 370, 207, makecol(255,0,255));
			rectfill(fundo, 384, 255, 418, 286, makecol(255,0,255));
			if (i==521)
				play_sample(explosao, 255, 128, 1000, 0);
		}
		if (i>=492 && i<512)
			rotate_sprite_v_flip(buffer, rocket[1], 300, RESOLUCAO_Y-rocket[1]->h, itofix(128));

		if (i>=495 && i<527) /* Missel 6 lan�ado */
		{
			rotate_sprite(buffer, missel, 290-(i-500)*2.7, 532-(i-495)*10, itofix(-85));
			if (i==495)
				play_sample(missile, 255/2, 128, 1000, 0);
		}

		if (i>=527 && i<547) /* Missel 6 explodindo */
		{
			draw_sprite(buffer, explode2[i-527], 170, 120);
			Explosao(fundo, 2, 100,80);
			rectfill(fundo, 200, 20, 280, 130, makecol(255,0,255));
			rectfill(fundo, 170, 195, 198, 272, makecol(255,0,255));
			if (i==527)
				play_sample(explosao, 255, 128, 1000, 0);
		}

		if (i>=512 && i<619)
			rotate_sprite_v_flip(buffer, rocket[0], 300, RESOLUCAO_Y-rocket[0]->h, itofix(128));
		
		if (i>=515 && i<550)/* Missel 7 lan�ado (eu juro que esse � o ultimo missel xD) */
		{
			rotate_sprite(buffer, missel, 290-(i-515)*6.5, 532-(i-515)*10, itofix(-96));
			if (i==515)
				play_sample(missile, 255/2, 128, 1000, 0);
		}

		if (i>=550 && i<570) /* Missel 7 explodindo */
		{
			draw_sprite(buffer, explode2[i-550], 20, 90);
			draw_sprite(buffer, explode2[i-550], 30, 95);
			draw_sprite(buffer, explode2[i-550], 40, 90);
			draw_sprite(buffer, explode2[i-550], 50, 95);
			Explosao(fundo, 4, -100,-50);
			rectfill(fundo, 0, 0, 73, 100, makecol(255,0,255));
			if (i==550)
				play_sample(explosao, 255, 128, 1000, 0);
		}

		if (i>=565 && i<=584)
			draw_sprite(buffer,anda[(i-565)%9], (i-565)*6, 500);
		
		if (i==584)
			stop_sample(passos);

		if (i>=584 && i<589) /* Tacow Berry tacando a bomba */
			draw_sprite(buffer,tacabomba[i-584], 114, 500);

		if (i>=584 && i<619) /* Bomba girando e caindo */
		{
			rotate_sprite(buffer, granada, k, (k-144)*(k-144)*0.002 + 500, itofix(k*3));
			k+=5;
		}

		if (i==619)
			play_sample(explosao, 255, 128, 1000, 0);
		if (i>=619 && i<639) /* ultima bomba explodindo */
			draw_sprite(buffer,explode[i-619], 300, RESOLUCAO_Y-explode[0]->h);

		if (i==625)
			play_sample(grito, 255, 128, 1000,0);

		if (i>=625 && i<645) /* Rocketeiro queimando */
			draw_sprite(buffer,queima[i-625], 285, RESOLUCAO_Y-queima[0]->h);
		
		if (i>=645) /* Esqueleto Roqueteiro */
			draw_sprite(buffer,queima[19], 285, RESOLUCAO_Y-queima[0]->h);
		
		if (i>=589 && i<610) /* Tacow Berry parado */
		{
			stop_sample(passos);
			draw_sprite(buffer,parado[((i-589)/4)%3], 114, 500);
		}

		if (i>=623) /* Aparecendo o "Orgulhosamente Apresenta" */
			blit(apresenta,buffer, 0,0, (RESOLUCAO_X-apresenta->w)/2, 510, (i-623)*6, apresenta->h);

		if (i==610)
			play_sample(passos, 255, 128, 1000, 1);

		if (i>=610) /* Tacow berry saindo de cena */
			draw_sprite(buffer,anda[(i-609)%9], (i-610)*6+114, 500);
			
		if (i>=610 && i<656) /* Mendigo passeando feliz e contente */
			rotate_sprite_v_flip(buffer,mendigoanda[((i-610)/2)%12], RESOLUCAO_X - (i-610)*2, 293, itofix(128));
		
		if (i>=625 && i<656) /* Bomba caindo na cabeca do mendigo */
			rotate_sprite(buffer, granada, 700, (i-625)*10, itofix((i-625)*3));
		
		if (i==656) /* Bomba explodindo */
			play_sample(explosao, 255, 128, 1000, 0);

		if (i>=656 && i<676)
		{
			draw_sprite(buffer,explode2[i-656], 670, 340-explode2[0]->h);
			Explosao(fundo, 3, 500, 130);
		}

		if (i==660)
			play_sample(grito, 255, 128, 1000,0);
		if (i>=660 && i<680)
			draw_sprite(buffer,queima[i-660], 670, 271);
		
		if (i>=680)
			draw_sprite(buffer,queima[19], 670, 271);
		draw_sprite(screen,buffer,0,0);
		if (i==725)
			stop_sample(passos);

		rest(37);
	}
	FadeOut(15);

	for (i=0; i<20 ;i++)
	{
		if (i==0)
		{
			destroy_bitmap(apresenta);
			destroy_bitmap(bomba);
			destroy_bitmap(fundo);
			destroy_bitmap(granada);
			destroy_bitmap(logotipo);
			destroy_bitmap(missel);
			destroy_sample(explosao);
			destroy_sample(grito);
			destroy_sample(missile);
			destroy_sample(passos);
			destroy_sample(recarrega1);
			destroy_sample(tank);
			destroy_sample(tiro);
		}

		if (i<2)
			destroy_bitmap(vira[i]);

		if (i<3)
		{
			destroy_bitmap(parado[i]);
			destroy_bitmap(rocket[i]);
		}

		if (i<4)
		{
			destroy_bitmap(abaixa[i]);
			destroy_bitmap(atira[i]);
		}
		if (i<5)
			destroy_bitmap(tacabomba[i]);

		if (i<7)
			destroy_bitmap(pula[i]);

		if (i<8)
			destroy_bitmap(mendigo[i]);

		if (i<9)
			destroy_bitmap(anda[i]);

		if (i<12)
		{
			destroy_bitmap(mendigoanda[i]);
			destroy_bitmap(tanque[i]);
		}

		if (i<19)
			destroy_bitmap(recarrega[i]);

		destroy_bitmap(explode[i]);
		destroy_bitmap(explode2[i]);
		destroy_bitmap(queima[i]);
	}
}

