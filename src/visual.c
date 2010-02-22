#include "gnd.h"


void EfeitoAbrir (BITMAP * bitmap, int comfundo, BITMAP * fundooo, int horizontalmente, int verticalmente, int pos_x, int pos_y, int velocidade)
{
	BITMAP* fundo;
	BITMAP* buffer = GetBuffer();
	int efeitovertical;
	float efeitohorizontal;
	fundo = create_bitmap(RESOLUCAO_X, RESOLUCAO_Y);
	if (comfundo==0)
	{
		BITMAP *fundoo;
		fundoo = create_bitmap(RESOLUCAO_X, RESOLUCAO_Y);
		clear_to_color(fundoo,0);
		draw_sprite(fundo, fundoo,0,0);	
	}
	else
		draw_sprite(fundo, fundooo,0,0);
	clear(buffer);
	clear_keybuf();
	while (mouse_b & 1);
	if (horizontalmente)
	{
		if (verticalmente)
		{
			for (efeitohorizontal=0, efeitovertical=0; efeitovertical<=RESOLUCAO_Y/2 && !keypressed() && !(mouse_b & 1); efeitohorizontal+=velocidade*((float)bitmap->w/(float)bitmap->h), efeitovertical+=velocidade)
			{
				show_mouse(NULL);
				draw_sprite(buffer,fundo,0,0);
				blit (bitmap,buffer, bitmap->w/2 - efeitohorizontal, bitmap->h/2-efeitovertical, bitmap->w/2 - efeitohorizontal + pos_x , (bitmap->h)/2 -efeitovertical + pos_y, efeitohorizontal*2,efeitovertical*2);
				if (efeitovertical+velocidade<=RESOLUCAO_Y/2)
					show_mouse(buffer);
				draw_sprite(screen, buffer,0,0);
				rest(1);
			}
		}
		else
		{
			for (efeitohorizontal=0; efeitohorizontal<=RESOLUCAO_X/2 && !keypressed() && !(mouse_b & 1); efeitohorizontal+=velocidade)
			{
				show_mouse(NULL);
				draw_sprite(buffer,fundo,0,0);
				blit (bitmap,buffer, bitmap->w/2 - efeitohorizontal, 0, (bitmap->w)/2 - efeitohorizontal + pos_x , pos_y, efeitohorizontal*2,bitmap->h);
				if (efeitohorizontal+velocidade<=RESOLUCAO_X/2)
					show_mouse(buffer);
				draw_sprite(screen, buffer,0,0);
				rest(1);
			}
		}
	}
	else
	{
		for (efeitovertical=0; efeitovertical<=RESOLUCAO_Y/2 && !keypressed() && !(mouse_b & 1); efeitovertical+=velocidade)
		{
			show_mouse(NULL);
			if (fundo)
				draw_sprite(buffer,fundo,0,0);
			draw_sprite(buffer,fundo,0,0);
			blit (bitmap,buffer, 0, bitmap->h/2 - efeitovertical, pos_x, (bitmap->h)/2 - efeitovertical + pos_y, bitmap->w, efeitovertical*2);
			if (efeitovertical+velocidade<=RESOLUCAO_Y/2)
			{
				show_mouse(buffer);
			}
			draw_sprite(screen, buffer,0,0);
			rest(1);
		}
	}
	show_mouse(NULL);
	draw_sprite(screen, bitmap, pos_x,pos_y);
	show_mouse(screen);
	while (mouse_b &1);
}


void FadeIn(BITMAP *bitmap, int velocidade)
{
	BITMAP* buffer = GetBuffer();
	int a;
	clear(buffer);
	for (a=0; a<256; a+=velocidade)
	{
		clear(buffer);
		set_trans_blender(0,0,0,a);
		draw_trans_sprite(buffer, bitmap, 0, 0);
		vsync();
		blit(buffer, screen, 0,0, 0,0, buffer->w, buffer->h);
		rest(10);
	}
	blit(bitmap, screen, 0,0, 0,0, bitmap->w, bitmap->h);
}


void Explosao (BITMAP*bitmap, float tamanho, int pos_x, int pos_y)
{
	int vertices[32] = {35*tamanho ,1*tamanho , 58*tamanho ,22*tamanho , 71*tamanho  ,1*tamanho,66*tamanho ,26*tamanho ,90*tamanho  ,15*tamanho  ,73*tamanho  ,42*tamanho  ,107*tamanho  ,64*tamanho  ,72*tamanho  ,68*tamanho  ,90*tamanho  ,104*tamanho  ,63*tamanho  ,85*tamanho  ,49*tamanho  ,102*tamanho  ,46*tamanho  ,63*tamanho  ,14*tamanho  ,72*tamanho  ,34*tamanho  ,49*tamanho  ,4*tamanho  ,39*tamanho  ,41*tamanho  ,31*tamanho };
	int cont;
	for (cont=0; cont<32 ; cont++)
	{
		if (cont%2)
			vertices[cont]+=pos_y;
		else
			vertices[cont]+=pos_x;
	}
	polygon (bitmap, 16, vertices, makecol(255,0,255));
}


void FadeOut(int velocidade)
{
	BITMAP *bmp_orig;
	BITMAP* buffer = GetBuffer();	
	int i;

	if ((bmp_orig = create_bitmap(RESOLUCAO_X, RESOLUCAO_Y)))
	{
		clear(buffer);
		blit(screen, bmp_orig, 0,0, 0,0, RESOLUCAO_X, RESOLUCAO_Y);
		if (velocidade <= 0)
			velocidade = 16;

		for (i = 255-velocidade; i > 0; i-=velocidade)
		{
			clear(buffer);
			set_trans_blender(0,0,0,i);
			draw_trans_sprite(buffer, bmp_orig, 0, 0);
			vsync();
			blit(buffer, screen, 0,0, 0,0, RESOLUCAO_X, RESOLUCAO_Y);
		}
		destroy_bitmap(bmp_orig);
	}
	rectfill(screen, 0,0, SCREEN_W,SCREEN_H, makecol(0,0,0));
}