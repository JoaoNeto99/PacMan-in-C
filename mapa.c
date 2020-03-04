#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define FPS 2.0
#define LARGURA_TELA 460
#define ALTURA_TELA 500

ALLEGRO_TIMER *timer = NULL;

ALLEGRO_DISPLAY *janela = NULL;

ALLEGRO_BITMAP *pacman = NULL;
ALLEGRO_BITMAP *pacmanR = NULL;
ALLEGRO_BITMAP *pacmanL = NULL;
ALLEGRO_BITMAP *pacmanU = NULL;
ALLEGRO_BITMAP *pacmanD = NULL;
ALLEGRO_BITMAP *shutup = NULL;

ALLEGRO_BITMAP *azul = NULL;
ALLEGRO_BITMAP *vermelho = NULL;
ALLEGRO_BITMAP *verde = NULL;
ALLEGRO_BITMAP *amarelo = NULL;

ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *bolas = NULL;

ALLEGRO_SAMPLE *inicio = NULL;
ALLEGRO_SAMPLE *comefruta = NULL;
ALLEGRO_SAMPLE *morreu = NULL;

ALLEGRO_FONT *fonte = NULL;
ALLEGRO_FONT *fonteGAMEOVER = NULL;

ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_EVENT evento;

char MAPA[24][24];

void error_msg(char *text);
int init();
void Constroimapa(int q);
void controle(int *i, int *j, int *dir);
void controlPacman(int *y, int *x, int tecla, int mov, int *anterior);
void MostraPontos(int pontos, int nivel);
void contaPontos(int y, int x, int *bola, int *pontos);
void ControlaFantasma(int *dirAmarelo, int *yAmarelo, int *xAmarelo, int codigo, int xPac, int yPac);
void destroi();
void abreFecha(int *mov);
int bola = 228, pontos = 0, nivel = 0;
int sair = 0;

int main(){
    int tecla = 0;
    int contador = 0;
   
    int mov = 1, x = 11, y = 17, anterior = 0;
    bool redraw = true, ganhou = false;

    int xAzul = 11, yAzul = 11;
    int xAmarelo = 11, yAmarelo = 8;
    int xVermelho = 11 , yVermelho = 10;
    int xVerde = 11, yVerde = 9;

    int dirVermelho = 2, dirAmarelo = 4, dirVerde = 1, dirAzul = 1;
    int codVermelho = 1, codAmarelo = 2, codVerde = 3, codAzul = 4;

    bool restart = false;
    float velocidade = 0.2;
        
    if (!init()){
        return -1;
    }
    Constroimapa(1); 
    
    al_play_sample(inicio, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);

    
    while (!sair){
        contador+=1;

        if(restart == true){
            contador = 0; 
            nivel += 1;
            al_draw_textf(fonteGAMEOVER, al_map_rgb(255, 255, 255), LARGURA_TELA/2, ALTURA_TELA/2, 5, "NIVEL %d", nivel);
            al_flip_display();
            al_rest(2.0);

            tecla = 0; 
            mov = 1, x = 11, y = 17, anterior = 0;

            xAzul = 11, yAzul = 11;
            xAmarelo = 11, yAmarelo = 8;
            xVermelho = 11 , yVermelho = 10;
            xVerde = 11, yVerde = 9;

            dirVermelho = 2, dirAmarelo = 4, dirVerde = 1, dirAzul = 1;
            codVermelho = 1, codAmarelo = 2, codVerde = 3, codAzul = 4;

            velocidade = velocidade - 0.05;

            bola = 228;
            
            Constroimapa(1);
            al_flip_display();
            restart = false;
        }
        al_draw_bitmap(fundo, 0, 0, 0);
        Constroimapa(0);
        al_draw_bitmap(pacman, x*20, y*20, 0);
        al_draw_bitmap(verde, xVerde*20, yVerde*20, 0);
        al_draw_bitmap(azul, xAzul*20, yAzul*20, 0);
        al_draw_bitmap(vermelho, xVermelho*20, yVermelho*20, 0);
        al_draw_bitmap(amarelo, xAmarelo*20, yAmarelo*20, 0);

        printf("cont: %d\n", contador);

        if(contador >= 20){
            ControlaFantasma(&dirAmarelo, &yAmarelo, &xAmarelo, codAmarelo, x,y);
        }
        if(contador >= 80){
            ControlaFantasma(&dirVermelho, &yVermelho, &xVermelho, codVermelho, x,y);
        }
        if(contador >= 130){
            ControlaFantasma(&dirAzul, &yAzul, &xAzul,codAzul, x,y);
        }
        if(contador >= 180){
            ControlaFantasma(&dirVerde, &yVerde, &xVerde, codVerde, x,y);
        }
        al_rest(velocidade);
      
       /*if(xAmarelo - 1 == x  && yAmarelo - 1 == y || xAmarelo + 1 == x  && yAmarelo + 1 == y ||
          xAzul - 1 == x && yAzul- 1 == y || xAzul + 1 == x && yAzul + 1 == y ||
          xVerde - 1 == x && yVerde - 1 == y || xVerde + 1 == x && yVerde + 1 == y ||
          xVermelho -1  == x && yVermelho - 1 == y ||  xVermelho + 1  == x && yVermelho + 1 == y){
            al_draw_textf(fonteGAMEOVER, al_map_rgb(255, 255, 255), LARGURA_TELA/2, ALTURA_TELA/2, 5, "GAME OVER");
            al_flip_display();
            al_rest(2);
            sair = 1;
        } */
        if(x == xAmarelo  && y == yAmarelo || x == xAzul && y == yAzul ||x == xVerde && y == yVerde || x == xVermelho && y == yVermelho){
            al_draw_textf(fonteGAMEOVER, al_map_rgb(255, 255, 255), LARGURA_TELA/2, ALTURA_TELA/2, 5, "GAME OVER");
            al_flip_display();
            al_rest(2);
            sair = 1;
        }
            
        al_flip_display(); 
        
        while (!al_is_event_queue_empty(fila_eventos)){  
            al_wait_for_event(fila_eventos, &evento); 
         
            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                sair = 1;
            }
            if(evento.type == ALLEGRO_EVENT_KEY_UP){
                switch (evento.keyboard.keycode){
                    case ALLEGRO_KEY_UP:                
                            anterior = tecla;
                            tecla = 1;     
                    break;
                    case ALLEGRO_KEY_DOWN: 
                           anterior = tecla;
                           tecla = 2;     
                    break;
                    case ALLEGRO_KEY_RIGHT: 
                            anterior = tecla;
                            tecla = 3;     
                    break;
                    case ALLEGRO_KEY_LEFT:
                            anterior = tecla;
                            tecla = 4;     
                       
                    break;
                    case ALLEGRO_KEY_ESCAPE:
                        sair = 1;
                    break;
                    default:
                    break;
                } 
            }                  
        }
        controlPacman(&y, &x, tecla, mov, &anterior);
        abreFecha(&mov);

        contaPontos(y,x, &bola, &pontos);
        printf("pontos: %d\n", pontos);
           
        if(bola == 0){
            restart = true;
        } 

        al_play_sample(comefruta, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL); 
        MostraPontos(pontos, nivel);
    }
    al_flip_display();
    destroi();
    return 0;
}

void destroi(){
    al_destroy_bitmap(pacman);
    al_destroy_bitmap(pacmanR);
    al_destroy_bitmap(pacmanD);
    al_destroy_bitmap(pacmanU);
    al_destroy_bitmap(pacmanL);
    al_destroy_bitmap(verde);
    al_destroy_bitmap(vermelho);
    al_destroy_bitmap(azul);
    al_destroy_bitmap(amarelo);

    al_destroy_bitmap(fundo);
    al_destroy_bitmap(bolas);
    al_destroy_timer(timer);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
}

void error_msg(char *text){
	al_show_native_message_box(NULL,"ERRO",
		"Ocorreu o seguinte erro e o programa sera finalizado:",
		text,NULL,ALLEGRO_MESSAGEBOX_ERROR);
}

void abreFecha(int *mov){
    if(*mov%2 == 0){
        pacman = shutup;
    }
    *mov= *mov + 1;
    if(*mov > 2 ) {*mov = 1;} 
}


void MostraPontos(int pontos, int nivel){
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_textf(fonte, al_map_rgb(158, 0, 255), 60, 460, 5, "Pontos:");   
    al_draw_textf(fonte, al_map_rgb(158, 0, 255), 150, 460, 5, "%d", pontos); 
    al_draw_textf(fonte, al_map_rgb(158, 0, 255), 240, 460, 5, "Nivel:"); 
    al_draw_textf(fonte, al_map_rgb(158, 0, 255), 290, 460, 5, "%d", nivel); 
    al_draw_textf(fonte, al_map_rgb(158, 0, 255), 430, 460, 5, "J.N");
}
void contaPontos(int y, int x, int *bola, int *pontos){
    if(MAPA[y][x] == '2'){
        MAPA[y][x] = '0';
        *bola=*bola - 1;
        *pontos=*pontos +1;
    }
}
void controlPacman(int *y, int *x, int tecla, int mov, int *anterior){
     if(tecla == 3 && *x >= 23){
        *y = 10;
        *x = 0;
    }
    if(tecla == 4 && *x <= 0){
        *y = 10;
        *x = 22;
    }
    contaPontos(*y, *x, &bola, &pontos);
    ////////////////////////////////////////////////
    //printf("tecla: %d \n", tecla);
    //printf("  ant:%d ", *anterior);

    if(tecla == 1 ){
        if(MAPA[*y-1][*x] != '1'){
            *y=*y-1;
            pacman = pacmanU; 
            *anterior = tecla;
        }else{
            if(*anterior == 4 && MAPA[*y][*x-1] != '1'){
                *x=*x-1;
                pacman = pacmanL; 
            }
            if(*anterior == 3 && MAPA[*y][*x+1] != '1'){
                *x=*x+1;
                pacman = pacmanR;
            }
        }
        contaPontos(*y, *x, &bola, &pontos);
    }
    if(tecla == 2 ){
        if(MAPA[*y+1][*x] != '1'){
            *y=*y+1;
            pacman = pacmanD;
            *anterior = tecla;
        }else{
            if(*anterior == 4 && MAPA[*y][*x-1] != '1'){
                *x=*x-1;
                pacman = pacmanL; 
            }
            if(*anterior == 3 && MAPA[*y][*x+1] != '1'){
                *x=*x+1;
                pacman = pacmanR;
            }
        }
        contaPontos(*y, *x, &bola, &pontos);
    }
    if(tecla == 3 ){
        if(MAPA[*y][*x+1] != '1'){
            *x=*x+1;
            pacman = pacmanR; 
            *anterior = tecla;
        }else{
            if(*anterior == 1 && MAPA[*y-1][*x] != '1'){
                *y=*y-1;
                pacman = pacmanU; 
            }
            if(*anterior == 2 && MAPA[*y+1][*x] != '1'){
                 *y=*y+1;
                pacman = pacmanD;
            }
        }
        contaPontos(*y, *x, &bola, &pontos);
    }
    if(tecla ==  4){
        if(MAPA[*y][*x-1] != '1'){
            *x=*x-1;
            pacman = pacmanL; 
            *anterior = tecla;
        }else{
            if(*anterior == 1 && MAPA[*y-1][*x] != '1'){
                *y=*y-1;
                pacman = pacmanU; 
            }
            if(*anterior == 2 && MAPA[*y+1][*x] != '1'){
                *y=*y+1;
                pacman = pacmanD;
            }
        }
        contaPontos(*y, *x, &bola, &pontos);
    }
   ////////////////////////////////////////////////
    

}

void Constroimapa(int q){
    int i, j;
    char mp[24][24] = {
       
            "11111111111111111111111",
            "12222222222122222222221",
            "12111211112121111211121",
            "12111211112121111211121",
            "12222222222222222222221",
            "12111212111111121211121",
            "12222212222122221222221",
            "11111211112121111211111",
            "11111212222222221211111",
            "11111212111011121211111",
            "22222222100000122222222",
            "11111212100000121211111",
            "11111212111011121211111",
            "11111212222222221211111",
            "11111212111111121211111",
            "12222222222122222222221",
            "12111211112121111211121",
            "12221222222022222212221",
            "11121212111111121212111",
            "12222212222122221222221",
            "12111111112121111111121",
            "12222222222222222222221",
            "11111111111111111111111",
        };

    for(i = 0; i < 24 ; i++){
        for(j = 0; j < 24; j++){
            if(q) MAPA[i][j] = mp[i][j];

            if(MAPA[i][j] == '2') al_draw_bitmap(bolas,j*20,i*20,0);
        }   
    }
}


void ControlaFantasma(int *direcao, int *y, int *x, int codigo, int xPac, int yPac){
    //fantasma vermelho
    if(codigo == 1 && *x < xPac && *direcao != 4 && MAPA[*y][*x+1] != '1') *direcao = 3;
        else if (codigo == 1 && *x > xPac && *direcao != 3 && MAPA[*y][*x-1] != '1') *direcao = 4;
        else if (codigo == 1 && *y < yPac && *direcao != 1 && MAPA[*y+1][*x] != '1') *direcao = 2;
        else if (codigo == 1 && *y > yPac && *direcao != 2 && MAPA[*y+1][*x] != '1') *direcao = 1;
    //fantasma azul
    if(codigo == 4 && *x < xPac-8 && *direcao != 4 && MAPA[*y][*x+1] != '1') *direcao = 3;
        else if (codigo == 3 && *x > xPac-8 && *direcao != 3 && MAPA[*y][*x-1] != '1') *direcao = 4;
        else if (codigo == 3 && *y < yPac-8 && *direcao != 1 && MAPA[*y+1][*x] != '1') *direcao = 2;
        else if (codigo == 3 && *y > yPac-8 && *direcao != 2 && MAPA[*y+1][*x] != '1') *direcao = 1;
    //fantasma verde
    if(codigo == 3 && *x < xPac+16 && *direcao != 4 && MAPA[*y][*x+1] != '1') *direcao = 3;
        else if (codigo == 3 && *x > xPac+16 && *direcao != 3 && MAPA[*y][*x-1] != '1') *direcao = 4;
        else if (codigo == 3 && *y < yPac+16 && *direcao != 1 && MAPA[*y+1][*x] != '1') *direcao = 2;
        else if (codigo == 3 && *y > yPac+16 && *direcao != 2 && MAPA[*y+1][*x] != '1') *direcao = 1;
    else{
        //Clyde, fantasmas amarelo que se move aleatoriamente
        ///direita == 3///
        if(*direcao == 3 && MAPA[*y][*x + 1] == '1'){//se o Clyde for para direita e na direita for parde
            if(MAPA[*y + 1][*x] != '1' && MAPA[*y - 1][*x] != '1'){//ele verifica se em cima ou se embaixo esta livre
                if(rand()%2 == 1){*direcao = 1;} else {*direcao = 2;} //ele sorteia um valor de 0 a 1 e se for 1 ele sobe senão ele desce
            }else{ //senão se para cima não for parece ele sobe senão ele desce
                if(MAPA[*y-1][*x] != '1'){*direcao = 1;} else{*direcao = 2;}
            }
        } 
        ///esquerda == 4///
        if(*direcao == 4 && MAPA[*y][*x - 1] == '1'){//se o Clyde for para esquerda e na direita for parede
            if(MAPA[*y + 1][*x] != '1' && MAPA[*y - 1][*x] != '1'){//ele verifica se em cima ou se embaixo esta livre
                if(rand()%2 == 1){*direcao = 1;} else {*direcao = 2;} //ele sorteia um valor de 0 a 1 e se for 1 ele sobe senão ele desce
            }else{ //senão se para cima não for parece ele sobe senão ele desce
                if(MAPA[*y-1][*x] != '1'){*direcao = 1;} else{*direcao = 2;}
            }
        }
        ///descer == 2///
        if(*direcao == 2 && MAPA[*y + 1][*x] == '1'){//se o Clyde for descer e na descer for parede
            if(MAPA[*y][*x + 1] != '1' && MAPA[*y][*x - 1] != '1'){//ele verifica se a direita ou se a esquerda esta livre
                if(rand()%2 == 1){*direcao = 4;} else {*direcao = 3;} //ele sorteia um valor de 0 a 1 e se for 1 ele vai para esquerda senão ele vai para direita
            }else{ //senão se para esquerda não for parece ele vai senão ele vai para direita
                if(MAPA[*y][*x - 1] != '1'){*direcao = 4;} else{*direcao = 3;}
            }
        } 
        ///subir == 1/// 
        if(*direcao == 1 && MAPA[*y - 1][*x] == '1'){//se o Clyde for descer e na descer for parede
            if(MAPA[*y][*x + 1] != '1' && MAPA[*y][*x - 1] != '1'){//ele verifica se a direita ou se a esquerda esta livre
                if(rand()%2 == 1){*direcao = 4;} else {*direcao = 3;} //ele sorteia um valor de 0 a 1 e se for 1 ele vai para esquerda senão ele vai para direita
            }else{ //senão se para esquerda não for parece ele vai senão ele vai para direita
                if(MAPA[*y][*x - 1] != '1'){*direcao = 4;} else{*direcao = 3;}
            }
        }
    }

        controle(x, y, direcao);

        if(xPac == *x  && yPac == *y){
            al_draw_textf(fonteGAMEOVER, al_map_rgb(255, 255, 255), LARGURA_TELA/2, ALTURA_TELA/2, 5, "GAME OVER");
            al_flip_display();
            al_rest(2);
            sair = 1;
        }


}

void controle(int *i, int *j, int *dir){
        if(*dir == 1 && MAPA[*j-1][*i] != '1'){
            *j=*j-1;
        }    
        if(*dir == 2 && MAPA[*j+1][*i] != '1' ){
            *j=*j+1;
        }
        if(*dir == 3 && MAPA[*j][*i+1] != '1' ){
            *i=*i+1;
        }
        if(*dir == 4 && MAPA[*j][*i-1] != '1' ){
            *i=*i-1;
        }
        if(*dir == 3 && *i >= 23){
            *j = 10;
            *i = 0;
        }
        if(*dir == 4 && *i <= -1){
            *j = 10;
            *i = 22;
        }   
        
}


int init(){
     //Inicialização da biblioteca Allegro
	if (!al_init()){
		error_msg("Falha ao inicializar a Allegro");
		return -1;
	}
 
	//tenta iniciar o módulo de imagens
	if (!al_init_image_addon()){
		error_msg("Falha ao inicializar add-on allegro_image");
		return -1;
	}

    //inicializa addon do teclado
    if (!al_install_keyboard()){
        error_msg("Falha ao inicializar o teclado");
        return 0;
    }

   //addon de audio
    if(!al_install_audio()){
        error_msg("Falha ao inicializar o audio");
        return 0;
    }
 
    //addon que da suporte as extensoes de audio
    if(!al_init_acodec_addon()){
        error_msg("Falha ao inicializar o codec de audio");
        return 0;
    }


    // Inicialização do add-on para uso de fontes
    if (!al_init_font_addon()){
        error_msg("Falha ao inicializar add-on allegro_font");
        return -1;
    }
 
    // Inicialização do add-on para uso de fontes True Type
    if (!al_init_ttf_addon()){
        error_msg("Falha ao inicializar add-on allegro_ttf");
        return -1;
    }

    // Carregando o arquivo de fonte
    fonte = al_load_font("Fontes/game.TTF", 20, 0);
    fonteGAMEOVER = al_load_font("Fontes/game.TTF", 50, 0);
    if (!fonte){
        al_destroy_display(janela);
        error_msg("Falha ao carregar fonte");
        return -1;
    }
 
    //cria o mixer (e torna ele o mixer padrao), e adciona 5 samples de audio nele
    if (!al_reserve_samples(2)){
        error_msg("Falha ao reservar amostrar de audio");
        return 0;
    }

    //morreu = al_load_sample("" );
    comefruta = al_load_sample("Sons/comefruta.wav" );
    inicio = al_load_sample("Sons/inicio.wav" );
    if(!comefruta){
        printf("comefruta audio clip sample not loaded! \n");
        return -1;
    }
    if(!inicio){
        printf("inicio audio clip sample not loaded! \n");
        return -1;
    }
 
	//cria display em janela
	janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
	//caso al_create_display retorne NULL, encerra programa
	if (!janela){
		error_msg("Falha ao criar janela");
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);
		return -1;
	}

    al_set_window_title(janela, "Pacman");

	//carrega pacman
    pacman = al_load_bitmap("Imagens/Pacman/pacman.png");
	//caso al_load_bitmap retorne NULL, encerra programa
	if (!pacman){
		error_msg("Falha ao carregar o arquivo de pacman");
		al_destroy_display(janela);
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);

		return -1;
	}

    shutup = al_load_bitmap("Imagens/Pacman/shutup.png");
	//caso al_load_bitmap retorne NULL, encerra programa
	if (!shutup){
		error_msg("Falha ao carregar o arquivo de shutup");
		al_destroy_display(janela);
        al_destroy_bitmap(pacman);
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);
		return -1;
	}

	pacmanR = al_load_bitmap("Imagens/Pacman/pac_right.png");
	//caso al_load_bitmap retorne NULL, encerra programa
	if (!pacmanR){
		error_msg("Falha ao carregar o arquivo de pacman");
		al_destroy_display(janela);
        al_destroy_bitmap(pacman);
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);
		return -1;
	}
    pacmanL = al_load_bitmap("Imagens/Pacman/pac_left.png");
	//caso al_load_bitmap retorne NULL, encerra programa
	if (!pacmanL){
		error_msg("Falha ao carregar o arquivo de pacman");
		al_destroy_display(janela);
        al_destroy_bitmap(pacman);
        al_destroy_bitmap(pacmanR);
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);
		return -1;
	}
    pacmanU = al_load_bitmap("Imagens/Pacman/pac_up.png");
	//caso al_load_bitmap retorne NULL, encerra programa
	if (!pacmanU){
		error_msg("Falha ao carregar o arquivo de pacman");
		al_destroy_display(janela);
        al_destroy_bitmap(pacman);
        al_destroy_bitmap(pacmanR);
        al_destroy_bitmap(pacmanL);
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);
		return -1;
	}
    pacmanD = al_load_bitmap("Imagens/Pacman/pac_down.png");
	//caso al_load_bitmap retorne NULL, encerra programa
	if (!pacmanD){
		error_msg("Falha ao carregar o arquivo de pacman");
		al_destroy_display(janela);
        al_destroy_display(janela);
        al_destroy_display(janela);
        al_destroy_bitmap(pacman);
        al_destroy_bitmap(pacmanR);
        al_destroy_bitmap(pacmanL);
        al_destroy_bitmap(pacmanD);
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);
		return -1;
	}

    fundo = al_load_bitmap("Imagens/Mapas/01/mapa1.png");
	//caso al_load_bitmap retorne NULL, encerra programa
	if (!fundo){
		error_msg("Falha ao carregar o arquivo de fundo");
		al_destroy_display(janela);
        al_destroy_bitmap(pacman);
        al_destroy_bitmap(pacmanR);
        al_destroy_bitmap(pacmanD);
        al_destroy_bitmap(pacmanU);
        al_destroy_bitmap(pacmanL);
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);
		return -1;
	}

    bolas = al_load_bitmap("Imagens/bolas/bolap.png");
	//caso al_load_bitmap retorne NULL, encerra programa
	if (!bolas){
		error_msg("Falha ao carregar o arquivo de bolas");
		al_destroy_display(janela);
       al_destroy_bitmap(pacman);
        al_destroy_bitmap(fundo);
        al_destroy_bitmap(pacmanR);
        al_destroy_bitmap(pacmanD);
        al_destroy_bitmap(pacmanU);
        al_destroy_bitmap(pacmanL);
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);
		return -1;
	}
    //Fantasmas
    azul = al_load_bitmap("Imagens/Fantasmas/azul.png");
	//azul
	if (!azul){
		error_msg("Falha ao carregar o arquivo de bolas");
		al_destroy_display(janela);
        al_destroy_bitmap(pacman);
        al_destroy_bitmap(fundo);
        al_destroy_bitmap(pacmanR);
        al_destroy_bitmap(pacmanD);
        al_destroy_bitmap(pacmanU);
        al_destroy_bitmap(pacmanL);
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);
        al_destroy_bitmap(bolas);
		return -1;
	}

    vermelho = al_load_bitmap("Imagens/Fantasmas/vermelho.png");
	//vermelho
	if (!vermelho){
		error_msg("Falha ao carregar o arquivo de bolas");
		al_destroy_display(janela);
        al_destroy_bitmap(pacman);
        al_destroy_bitmap(fundo);
        al_destroy_bitmap(pacmanR);
        al_destroy_bitmap(pacmanD);
        al_destroy_bitmap(pacmanU);
        al_destroy_bitmap(pacmanL);
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);
        al_destroy_bitmap(bolas);
        al_destroy_bitmap(azul);
		return -1;
	}

    verde = al_load_bitmap("Imagens/Fantasmas/verde.png");
	//verde
	if (!verde){
		error_msg("Falha ao carregar o arquivo de bolas");
		al_destroy_display(janela);
        al_destroy_bitmap(pacman);
        al_destroy_bitmap(fundo);
        al_destroy_bitmap(pacmanR);
        al_destroy_bitmap(pacmanD);
        al_destroy_bitmap(pacmanU);
        al_destroy_bitmap(pacmanL);
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);
        al_destroy_bitmap(bolas);
        al_destroy_bitmap(azul);
        al_destroy_bitmap(vermelho);
		return -1;
	}

    amarelo = al_load_bitmap("Imagens/Fantasmas/amarelo.png");
	//amarelo
	if (!verde){
		error_msg("Falha ao carregar o arquivo de bolas");
		al_destroy_display(janela);
        al_destroy_bitmap(pacman);
        al_destroy_bitmap(fundo);
        al_destroy_bitmap(pacmanR);
        al_destroy_bitmap(pacmanD);
        al_destroy_bitmap(pacmanU);
        al_destroy_bitmap(pacmanL);
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);
        al_destroy_bitmap(bolas);
        al_destroy_bitmap(azul);
        al_destroy_bitmap(vermelho);
        al_destroy_bitmap(verde);

		return -1;
	}

    timer = al_create_timer(1.0/FPS);
    if(!timer){
        error_msg("Falha ao inicializar o temporizador");
        return 0;
    }
 
	//cria fila de eventos
	fila_eventos = al_create_event_queue();
	//caso al_create_event_queue retorne NULL, destroi a janela e encerra o programa
	if (!fila_eventos){
		error_msg("Falha ao criar fila de eventos");
		al_destroy_display(janela);
        al_destroy_bitmap(pacman);
        al_destroy_bitmap(fundo);
        al_destroy_bitmap(pacmanR);
        al_destroy_bitmap(pacmanD);
        al_destroy_bitmap(pacmanU);
        al_destroy_bitmap(pacmanL);
        al_destroy_sample(comefruta);
        al_destroy_sample(inicio);
        al_destroy_bitmap(bolas);
        al_destroy_bitmap(azul);
        al_destroy_bitmap(vermelho);
        al_destroy_bitmap(verde);
        al_destroy_bitmap(amarelo);
		return -1;
	}

    //registra duas fontes de eventos na fila. o da janela, e do teclado
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    al_flip_display();
    

}