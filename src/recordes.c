#include "gnd.h"


int EntrouNosRecordes(int pontos, char nomes[10][21], int rec_pontos[10])
{
	int i;
	CarregaRecordes(nomes, rec_pontos);
	for(i=0;i<10;i++)
		if(pontos > rec_pontos[i]) return 1;

	return 0;
}


void AdicionaLetraAoNome(char nome[21], char letra)
{
	if(strlen(nome) > 19) return;

	nome[strlen(nome)] = letra;
	nome[strlen(nome) + 1] = 0;
}


void RemoveUltimaLetraDoNome(char nome[21])
{
	if((strlen(nome) - 1) < 0) return;

	nome[strlen(nome) - 1] = 0;
}


void AtualizaNome(char nome[21], BITMAP *buffer, BITMAP *recorde, BITMAP *letras[26])
{
	int i;
	blit(recorde, buffer, 0, 0, GAMEOVER_RECORDE_X, GAMEOVER_RECORDE_Y, recorde->w, recorde->h);
	
	for(i=0;nome[i]!= 0;i++)
	{
		draw_sprite(buffer, letras[nome[i] - 'A'], GAMEOVER_RECORDE_NOME_X + LETRAS_LARG * i, GAMEOVER_RECORDE_NOME_Y);
	}
}


void CarregaRecordes(char nomes[10][21], int rec_pontos[10])
{
	FILE *recordes;
	int i;

	recordes = fopen("recordes.rec", "r");
	if(!recordes) /* Nao achou o arquivo, ou nao pode acessa-lo */
	{
		/* Supondo que nao tenha achado, vamos tentar cria-lo */
		//fclose(recordes);
		recordes = fopen("recordes.rec", "w");
		if(!recordes) /* Tambem nao pode criar... problemas... */
		{
			printf("Arquivo recordes.rec nao pode ser aberto nem criado!\n");
		}
		
		for(i=0;i<10;i++)
		{
			nomes[i][0] = 0;
			rec_pontos[i] = 0;
		}
		
		fclose(recordes);
		return;
	}

	fseek(recordes, 0, SEEK_SET);
	for(i=0;i<10;i++)
	{
		if((fread(&nomes[i][0], sizeof(char), 21, recordes) != 21) || (fread(&rec_pontos[i], sizeof(int), 1, recordes) != 1))
		{
			printf("Problema ao ler o arquivo recordes.rec\n");
			break;
		}
	}
	
	fclose(recordes);
}


void GravaOrdenado(char nomes[10][21], int rec_pontos[10])
{
	FILE *recordes;
	int i, j, maior = 0, maior_ind, gravado[10], total = 0;

	recordes = fopen("recordes.rec", "w");
	if(!recordes) /* Tambem nao pode criar... problemas... */
	{
		printf("Arquivo recordes.rec n�o pode ser aberto nem criado!\n");
		return;
	}

	fseek(recordes, 0, SEEK_SET);

	for(i=0;i<10;i++) gravado[i] = 0;

	while(total < 10)
	{
		maior = 0;
		maior_ind = 0;
		
		for(i=0;i<10;i++)
		{
			if(rec_pontos[i] >= maior && gravado[i] == 0)
			{
				maior = rec_pontos[i];
				maior_ind = i;
			}
		}

		gravado[maior_ind] = 1;

		for(j=0;j<21;j++)
			fwrite(&nomes[maior_ind][j], sizeof(char), 1, recordes);

		fwrite(&rec_pontos[maior_ind], sizeof(int), 1, recordes);

		/*if(fwrite(&nomes[maior_ind][0], sizeof(char), 21, recordes) != 21 || fwrite(&rec_pontos[maior_ind], sizeof(int), 1, recordes) != 1)
		{
			printf("Problema ao escrever o arquivo recordes.rec\n");
			break;
		}*/
		total++;
	}

    	fclose(recordes);
}


void InsereRecorde(char nome[21], int pontos, char rec_nomes[10][21], int rec_pontos[10])
{
	int i, menor = rec_pontos[0], ind_menor = 0;

	for(i=1;i<10;i++) if(rec_pontos[i] < menor) ind_menor = i;

	rec_pontos[ind_menor] = pontos;
	
	for(i=0;i<21;i++) rec_nomes[ind_menor][i] = nome[i];
}


int GameOverEstaNoOk()
{
	if(mouse_x >= GAMEOVER_BOTAO_OK_X &&
		mouse_x <= (GAMEOVER_BOTAO_OK_X + GAMEOVER_BOTAO_OK_LARG) &&
		mouse_y >= GAMEOVER_BOTAO_OK_Y &&
		mouse_y <= (GAMEOVER_BOTAO_OK_Y + GAMEOVER_BOTAO_OK_ALT))
			return 1;

	return 0;
}


void GameOver(int pontos)
{
	BITMAP *buffer = create_bitmap(800, 600);
	BITMAP *fundo = load_bitmap(PATH_GAMEOVER_SCREEN, NULL);
	BITMAP *recorde = load_bitmap(PATH_GAMEOVER_RECORDE, NULL);
	BITMAP *letras[26];
	BITMAP *botao[2];
	int i,j;
	int rec_pontos[10];
	int botao_ok = 0;
	char nome[21];
	char rec_nomes[10][21];
	//char scan_char;

	while ((mouse_b & 1) || keypressed());

	botao[1] = load_bitmap(PATH_GAMEOVER_OK, NULL);
	botao[0] = create_bitmap(GAMEOVER_BOTAO_OK_LARG, GAMEOVER_BOTAO_OK_ALT);

	for(i=0;i<21;i++) nome[i] = 0;
	for(i=0;i<10;i++) rec_pontos[i] = 0;

	for(i=0;i<10;i++)
		for(j=0;j<21;j++) rec_nomes[i][j] = 0;

	if(!fundo)
	{
		printf("Erro ao carregar o bitmap gameover.tga\n");
		exit(1);
	}
	if(!recorde)
	{
		printf("Erro ao carregar o bitmap recorde.tga\n");
		exit(1);
	}
	if(!buffer)
	{
		printf("Erro ao criar o buffer.\n");
		exit(1);
	}
	if(!botao[0])
	{
		printf("Erro ao criar o botao[0].\n");
		exit(1);
	}
	if(!botao[1])
	{
		printf("Erro ao criar o botao[0].\n");
		exit(1);
	}

	CarregaRecordes(rec_nomes, rec_pontos);
	SeparaFrames(PATH_LETRAS, letras, 26);
	DrawBackground(fundo, buffer);

	if(!EntrouNosRecordes(pontos, rec_nomes, rec_pontos))
	{
		show_mouse(NULL);
		BufferToScreen(buffer, screen);
		show_mouse(screen);
		clear_keybuf();
		while(!key[KEY_ESC] && !mouse_b);
		clear_keybuf();
		return;
	}

	blit(recorde, buffer, 0, 0, GAMEOVER_RECORDE_X, GAMEOVER_RECORDE_Y, recorde->w, recorde->h);
	blit(buffer, botao[0], GAMEOVER_BOTAO_OK_X, GAMEOVER_BOTAO_OK_Y, 0, 0, GAMEOVER_BOTAO_OK_LARG, GAMEOVER_BOTAO_OK_ALT);
	show_mouse(NULL);
	BufferToScreen(buffer, screen);
	show_mouse(screen);
	clear_keybuf();

	while(1)
	{
		while(keypressed() || mouse_b)
		{
			if(GameOverEstaNoOk() && botao_ok == 0)
			{
				blit(botao[1], buffer, 0, 0, GAMEOVER_BOTAO_OK_X, GAMEOVER_BOTAO_OK_Y, GAMEOVER_BOTAO_OK_LARG, GAMEOVER_BOTAO_OK_ALT);
				show_mouse(NULL);
				BufferToScreen(buffer, screen);
				show_mouse(screen);
				botao_ok = 1;
			}
			else if(botao_ok == 1 && !GameOverEstaNoOk())
			{
				blit(botao[0], buffer, 0, 0, GAMEOVER_BOTAO_OK_X, GAMEOVER_BOTAO_OK_Y, GAMEOVER_BOTAO_OK_LARG, GAMEOVER_BOTAO_OK_ALT);
				show_mouse(NULL);
				BufferToScreen(buffer, screen);
				show_mouse(screen);
				botao_ok = 0;
			}
			
			rest(1);
		}
		
		while(!keypressed() && !mouse_b)
		{
			if(GameOverEstaNoOk() && botao_ok == 0)
			{
				blit(botao[1], buffer, 0, 0, GAMEOVER_BOTAO_OK_X, GAMEOVER_BOTAO_OK_Y, GAMEOVER_BOTAO_OK_LARG, GAMEOVER_BOTAO_OK_ALT);
				show_mouse(NULL);
				BufferToScreen(buffer, screen);
				show_mouse(screen);
				botao_ok = 1;
			}
			else if(botao_ok == 1 && !GameOverEstaNoOk())
			{
				blit(botao[0], buffer, 0, 0, GAMEOVER_BOTAO_OK_X, GAMEOVER_BOTAO_OK_Y, GAMEOVER_BOTAO_OK_LARG, GAMEOVER_BOTAO_OK_ALT);
				show_mouse(NULL);
				BufferToScreen(buffer, screen);
				show_mouse(screen);
				botao_ok = 0;
			}
			
			rest(1);
		}

		if(mouse_b && GameOverEstaNoOk()) break;

		if(key[KEY_BACKSPACE]) RemoveUltimaLetraDoNome(nome);
		else if(key[KEY_ENTER]) break;
		else if(key[KEY_ESC]) break;

		/*scan_char = readkey() & 0xff;*/
		if(key[KEY_A])      AdicionaLetraAoNome(nome, 'A');
		else if(key[KEY_B]) AdicionaLetraAoNome(nome, 'B');
		else if(key[KEY_C]) AdicionaLetraAoNome(nome, 'C');
		else if(key[KEY_D]) AdicionaLetraAoNome(nome, 'D');
		else if(key[KEY_E]) AdicionaLetraAoNome(nome, 'E');
		else if(key[KEY_F]) AdicionaLetraAoNome(nome, 'F');
		else if(key[KEY_G]) AdicionaLetraAoNome(nome, 'G');
		else if(key[KEY_H]) AdicionaLetraAoNome(nome, 'H');
		else if(key[KEY_I]) AdicionaLetraAoNome(nome, 'I');
		else if(key[KEY_J]) AdicionaLetraAoNome(nome, 'J');
		else if(key[KEY_K]) AdicionaLetraAoNome(nome, 'K');
		else if(key[KEY_L]) AdicionaLetraAoNome(nome, 'L');
		else if(key[KEY_M]) AdicionaLetraAoNome(nome, 'M');
		else if(key[KEY_N]) AdicionaLetraAoNome(nome, 'N');
		else if(key[KEY_O]) AdicionaLetraAoNome(nome, 'O');
		else if(key[KEY_P]) AdicionaLetraAoNome(nome, 'P');
		else if(key[KEY_Q]) AdicionaLetraAoNome(nome, 'Q');
		else if(key[KEY_R]) AdicionaLetraAoNome(nome, 'R');
		else if(key[KEY_S]) AdicionaLetraAoNome(nome, 'S');
		else if(key[KEY_T]) AdicionaLetraAoNome(nome, 'T');
		else if(key[KEY_U]) AdicionaLetraAoNome(nome, 'U');
		else if(key[KEY_V]) AdicionaLetraAoNome(nome, 'V');
		else if(key[KEY_W]) AdicionaLetraAoNome(nome, 'W');
		else if(key[KEY_X]) AdicionaLetraAoNome(nome, 'X');
		else if(key[KEY_Y]) AdicionaLetraAoNome(nome, 'Y');
		else if(key[KEY_Z]) AdicionaLetraAoNome(nome, 'Z');

		show_mouse(NULL);
		AtualizaNome(nome, buffer, recorde, letras);
		BufferToScreen(buffer,screen);
		show_mouse(screen);
		clear_keybuf();
	}

	while(mouse_b);

	destroy_bitmap(buffer);
	destroy_bitmap(fundo);
	destroy_bitmap(recorde);
	destroy_bitmap(botao[0]);
	destroy_bitmap(botao[1]);
	
	for(i=0;i<26;i++) destroy_bitmap(letras[i]);

	InsereRecorde(nome, pontos, rec_nomes, rec_pontos);
	GravaOrdenado(rec_nomes, rec_pontos);
}