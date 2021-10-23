
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stack>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <list>
#include <random>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iostream>


#include <string.h>
#include <SDL2/SDL_ttf.h>
#include "tinyexpr.h"


using namespace std;


const char* nomeFonte = "arial.ttf";
const int sizeFonte = 14;
const int metadeSizeFonte = (sizeFonte/2);
SDL_Surface *text_surface;




// Variável para "x"
double x_variable = 0;
// Controle de erros
int err;
// Ponteiro para expressão compilada
te_expr *expr;




// Estrutura para representar pontos
typedef struct {int x, y;} Point;

// Estrutura para representar coordenada
typedef struct {double x, y;} Coordenada;

typedef struct {
    std::string espressao ;
    Uint32 color;
} Funcao;

// Variáveis necessárias para o SDL
unsigned int * pixels;
int width, height;
SDL_Surface * window_surface;
SDL_Renderer * renderer;

TTF_Font *font;

// Título da janela
std::string titulo = "Tarefa 1";

// Valores RGB para a cor de fundo da janela
const int VERMELHO = 0;
const int VERDE = 0;
const int AZUL = 0;

// Tamanho da janela principal
const int window_width = 800;
const int window_height = 600;


// Solicita ao usuário que informe uma função
string obtemFuncao()
{
    string expression_string = "";

    while(expression_string == ""){
        cout<<"Informe a equacao a ser desenhada: ";
        getline(cin, expression_string);

        if(expression_string == ""){
            cout<<"ERRO. Informe um valor valido!\n";
        }
    }


    cout<<"A equacao sera : " << expression_string << endl;
    return expression_string;
}

int NumeroRandomico(int min, int max){

    srand((unsigned)time(0));
    return (rand()%max)+min;
    /*
    cout << i << "\n";


    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min,max); // distribution in range [1, 6]

    std::cout << dist6(rng) << std::endl;
    */
}

// Compila a expressão e registra as variáveis
void preparaFuncao(string expression_string)
{
    // Vincula o valor da variável global "x_variable" à variável "x" da expressão
    te_variable vars[] = {"x", &x_variable};

    /* Compile the expression with variables. */
    expr = te_compile(expression_string.c_str(), vars, 1, &err);

}

// Gera uma estrutura Point a partir de valores para x e y
Point getPoint(int x, int y)
{
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

// Gera uma estrutura Coordenada a partir de valores para x e y
Coordenada getCoordenada(double x, double y)
{
    Coordenada c;
    c.x = x;
    c.y = y;
    return c;
}

// Gera uma estrutura Funcao a partir de valores
Funcao getFuncao(std::string espressao, Uint32 color)
{
    Funcao f;
    f.espressao = espressao;
    f.color = color;
    return f;
}


// Obtém a cor de um pixel de uma determinada posição
Uint32 getPixel(int x, int y)
{
    if((x>=0 && x<=width) && (y>=0 && y<=height))
        return pixels[x + width * y];
    else
        return -1;
}

// Seta um pixel em uma determinada posição,
// através da coordenadas de cor r, g, b, e alpha (transparência)
// r, g, b e a variam de 0 até 255
void setPixel(int x, int y, int r, int g, int b, int a)
{
    pixels[x + y * width] = SDL_MapRGBA(window_surface->format, r, g, b, a);
}

// Seta um pixel em uma determinada posição,
// através da coordenadas de cor r, g e b
// r, g, e b variam de 0 até 255
void setPixel(int x, int y, int r, int g, int b)
{
    setPixel(x, y, r, g, b, 255);
}



// Imprime na console a posição corrente do mouse
void printMousePosition(int x, int y)
{
    printf("Mouse on x = %d, y = %d\n",x,y);
}

// Retorna uma cor RGB(UInt32) formada
// pelas componentes r, g, b e a(transparência)
// informadas. r, g, b e a variam de 0 até 255
Uint32 RGB(int r, int g, int b, int a) {
    return SDL_MapRGBA(window_surface->format, r, g, b, a);
}

// Retorna uma cor RGB(UInt32) formada
// pelas componentes r, g, e b
// informadas. r, g e b variam de 0 até 255
// a transparência é sempre 255 (imagem opaca)
Uint32 RGB(int r, int g, int b) {
    return SDL_MapRGBA(window_surface->format, r, g, b, 255);
}

// Retorna um componente de cor de uma cor RGB informada
// aceita os parâmetros 'r', 'R','g', 'G','b' e 'B'
Uint8 getColorComponent( Uint32 pixel, char component ) {

    Uint32 mask;

    switch(component) {
        case 'b' :
        case 'B' :
            mask = RGB(0,0,255);
            pixel = pixel & mask;
            pixel = pixel >> 24;
            break;
        case 'r' :
        case 'R' :
            mask = RGB(255,0,0);
            pixel = pixel & mask;
            pixel = pixel >> 16;
            break;
        case 'g' :
        case 'G' :
            mask = RGB(0,255,0);
            pixel = pixel & mask;
            pixel = pixel >> 8;
            break;
    }
    return (Uint8) pixel;
}

// Seta um pixel em uma determinada posição,
// através de um Uint32 representando
// uma cor RGB
void setPixel(int x, int y, Uint32 color)
{
    if((x<0 || x>=width || y<0 || y>=height)) {
        //printf("Coordenada inválida : (%d,%d)\n",x,y);
        return;
    }

    pixels[x + y * width] = color;
}



float modulo(float x){
    if(x < 0){
        return -x;
    }
    return x;
}

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int iPartOfNumber(float x){
    return (int)x;
}

float fPartOfNumber(float x){
    if(x > 0){
        return x - iPartOfNumber(x);
    }
    return x - (iPartOfNumber(x) + 1);
}

float rfPartOfNumber(float x){
    return 1 - fPartOfNumber(x);
}

void drawPixel(int x, int y, int red, int green, int blue, float brightness){

    int temp = 255 * brightness;
    setPixel( x, y, temp, temp, temp);

}

void printSimpleHorizontalLine(int xi, int xf, int y, Uint32 color){
    if(xf < xi){
        int temp = xi;
        xi = xf;
        xf= temp;
    }

    for(int x = xi; x <= xf; x++){
        setPixel(x, y, color);
    }

}

void printSimpleVerticalLine(int yi, int yf, int x, Uint32 color){
    if(yf < yi){
        int temp = yi;
        yi = yf;
        yf= temp;
    }

    for(int y = yi; y <= yf; y++){
        setPixel(x, y, color);
    }

}

void xiaolinwuLine(int x0, int y0, int x1, int y1, int red, int green, int blue){
    int steep = modulo(y1 - y0) > modulo(x1 - x0);

    if(steep){
        swap(&x0, &y0);
        swap(&x1, &y1);
    }

    if(x0 > x1){
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    float dx = x1 - x0;
    float dy = y1 - y0;
    float gradiente = dy/dx;

    if(dx == 0.0){
        gradiente = 1;
    }

    int xpxl1 = x0;
    int xpxl2 = x1;
    float intersectY = y0;

    int x;

    if(steep){

        for(x = xpxl1; x <= xpxl2; x++){
            drawPixel(iPartOfNumber(intersectY), x, red, green, blue, rfPartOfNumber(intersectY));
            drawPixel(iPartOfNumber(intersectY)-1, x, red, green, blue, fPartOfNumber(intersectY));

            intersectY += gradiente;
        }

    }else{

        for(x = xpxl1; x <= xpxl2; x++){
            drawPixel(x, iPartOfNumber(intersectY), red, green, blue, rfPartOfNumber(intersectY));
            drawPixel(x, iPartOfNumber(intersectY)-1, red, green, blue, fPartOfNumber(intersectY));

            intersectY += gradiente;
        }

    }

}

void bresenhamLine(int x1, int y1, int x2, int y2, Uint32 color){

    int slope; // inclinação
    int dx, dy, incE, incNE, d, x, y;
    // Onde inverte a linha x1 > x2
    if (x1 > x2){
        bresenhamLine(x2, y2, x1, y1, color);
        return;
    }
    dx = x2 - x1;
    dy = y2 - y1;

    if (dy < 0){
        slope = -1;
        dy = -dy;
    }
    else{
        slope = 1;
    }

    // Constante de Bresenham
    incE = 2 * dy; // shift-left
    incNE = 2 * dy - 2 * dx;
    d = 2 * dy - dx;
    y = y1;
    for (x = x1; x <= x2; x++){
        setPixel( x, y, color);
        if (d <= 0){
            d += incE;
        }
        else{
            d += incNE;
            y += slope;
        }
    }

}

void printLine(int xi, int yi, int xf, int yf, int red, int green, int blue, int a, bool anti_aliasing){

    if(anti_aliasing){
        xiaolinwuLine(xi, yi, xf, yf, red, green, blue);

    }else{
        bresenhamLine(xi, yi, xf, yf, RGB(red, green, blue, a));

    }

}

void displayBresenhamCircle(int xc, int yc, int x, int y, Uint32 color){

    setPixel(xc + x, yc + y, color);
    setPixel(xc - x, yc + y, color);
    setPixel(xc + x, yc - y, color);
    setPixel(xc - x, yc - y, color);

    setPixel(xc + y, yc + x, color);
    setPixel(xc - y, yc + x, color);
    setPixel(xc + y, yc - x, color);
    setPixel(xc - y, yc - x, color);
}

void printBresenhamCircle(int xc, int yc, int radius, Uint32 color){

    int x = 0, y = radius;
    int decesionParameter = 3 - 2 * radius;
    displayBresenhamCircle(xc, yc, x, y, color);

    while(y >= x){

        x++;
        if(decesionParameter > 0){
            y--;
            decesionParameter += 4 * (x - y) + 10;
        }else{
            decesionParameter += 4 * x + 6;
        }

        displayBresenhamCircle(xc, yc, x, y, color);

    }

}

void printBezierCurve(Point p0, Point p1, Point p2, Point p3, Uint32 color){

    int x, y;
    for(float u = 0; u <= 1; u += 0.0001){

        x = pow(1-u,3) * p0.x + 3 * u *
            pow(1-u,2) * p1.x + 3 * u * u *
            (1-u) * p2.x + u * u * u * p3.x;

        y = pow(1-u,3) * p0.y + 3 * u *
            pow(1-u,2) * p1.y + 3 * u * u *
            (1-u) * p2.y + u * u * u * p3.y;

        setPixel(x, y, color);

    }

}

void floodFill(int x, int y, Uint32 newColor, Uint32 oldColor){

    if(y<0 || y > height-1 || x<0 || x> width-1){
        return;
    }

    stack<Point> st;
    st.push(getPoint(x,y));
    while(st.size() > 0){

        Point p = st.top();
        st.pop();
        int x = p.x;
        int y = p.y;
        if(y<0 || y > height-1 || x<0 || x> width-1){
            continue;
        }
        if(getPixel(x,y) == oldColor){
            setPixel(x, y, newColor);
            st.push(getPoint(x+1, y));
            st.push(getPoint(x-1, y));
            st.push(getPoint(x, y+1));
            st.push(getPoint(x, y-1));
        }
    }

}

void printCoracao(int x, int y, int altura, int largura, Uint32 color){

    Point pCima;
    pCima.x = x;
    pCima.y = y - altura/2 + (altura/4);

    Point pBaixo;
    pBaixo.x = x;
    pBaixo.y = y + altura/2;

    Point pEsquerdoCima;
    pEsquerdoCima.x = x - largura/2;
    pEsquerdoCima.y = pCima.y - altura/3;

    Point pEsquerdoBaixo;
    pEsquerdoBaixo.x = x - largura/2;
    pEsquerdoBaixo.y = pBaixo.y - altura/4;



    Point pDireitoCima;
    pDireitoCima.x = x + largura/2;
    pDireitoCima.y = pEsquerdoCima.y;

    Point pDireitoBaixo;
    pDireitoBaixo.x = x + largura/2;
    pDireitoBaixo.y = pEsquerdoBaixo.y;




    printBezierCurve(pCima, pEsquerdoCima, pEsquerdoBaixo, pBaixo, color);

    printBezierCurve(pCima, pDireitoCima, pDireitoBaixo, pBaixo, color);





}

void printRetangulo(Point p1, Point p2, bool innerColor, Uint32 color){

    printSimpleHorizontalLine(p1.x, p2.x, p1.y, color);
    printSimpleHorizontalLine(p1.x, p2.x, p2.y, color);

    printSimpleVerticalLine(p1.y, p2.y, p1.x, color);
    printSimpleVerticalLine(p1.y, p2.y, p2.x, color);

    if(innerColor){
        floodFill(p2.x-p1.x, p2.y-p1.y, color, RGB(VERMELHO, VERDE, AZUL));
    }

}



//____________________________________________________________________________________________________


double delta = 1.0 / 1000.0;

// a taxa de mudanca é o valor que irá alterar
// as coordenadas mínimas e máxima+-+-*+-

//*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// para o trabalho em questão o valor deveria ser de: 1;
// mas para melhorar a animação esse valor poderá ser alterado;
double taxaMudanca = 0.25;

const Point graph_min = getPoint(125, 25);
const Point graph_max = getPoint(675, 575);

Point cursor = getPoint(0,0);

Coordenada coord_min = getCoordenada(-10,-10);
Coordenada coord_max = getCoordenada(10,10);

//LISTAFUNCAO funcoes;

std::list<Funcao> funcoes = { };

bool isInside(Coordenada c1, Coordenada coord_min, Coordenada coord_max){
    return (c1.x >= coord_min.x && c1.x <= coord_max.x && c1.y >= coord_min.y && c1.y <= coord_max.y);
}

Coordenada getCoordenadaFromPoint(Point ponto){

    if(ponto.x < graph_min.x || ponto.x > graph_max.x || ponto.y < graph_min.y || ponto.y > graph_max.y){
        return getCoordenada(coord_max.x - coord_min.x, coord_max.y - coord_min.y);
    }

    float x = ((float)ponto.x - graph_min.x)/(graph_max.x - graph_min.x);
    float y = ((float)graph_max.y - ponto.y)/(graph_max.y - graph_min.y);

    Coordenada coord = getCoordenada((coord_max.x - coord_min.x) * x + coord_min.x, (coord_max.y - coord_min.y) * y + coord_min.y);

    return coord;

}

void DesenhaPontoMapeado(Coordenada ponto, Uint32 color){

    if(isInside(ponto, coord_min, coord_max)){
        Coordenada coord_temp = getCoordenada(coord_max.x - coord_min.x, coord_max.y - coord_min.y);

        double proporcao_x = (-coord_min.x)/coord_temp.x;
        double proporcao_y = (-coord_min.y)/coord_temp.y;

        Coordenada pontoMapeado = getCoordenada(ponto.x+coord_temp.x * proporcao_x, coord_temp.y -(ponto.y + coord_temp.y* proporcao_y));

        pontoMapeado.x = pontoMapeado.x * (graph_max.x - graph_min.x) / coord_temp.x + graph_min.x;
        pontoMapeado.y = pontoMapeado.y * (graph_max.y - graph_min.y) / coord_temp.y + graph_min.y;
        setPixel(pontoMapeado.x, pontoMapeado.y, color);
    }
}


void LinhaCartesianaHorizontal(double coord_xi, double coord_xf, int coord_y, Uint32 color){

    for(double x = coord_xi; x <= coord_xf; x+=delta){
        DesenhaPontoMapeado(getCoordenada(x, coord_y), color);
    }

}

void LinhaCartesianaVertical(double coord_yi, double coord_yf, int coord_x, Uint32 color){

    for(double y = coord_yi; y <= coord_yf; y+=delta){
        DesenhaPontoMapeado(getCoordenada(coord_x, y), color);
    }

}




void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
    SDL_FreeSurface(text_surface);
}

void PrintText(int x, int y, char * texto, Uint32 color){

    SDL_Color fonteColor={getColorComponent(color, 'r'), getColorComponent(color, 'g'), getColorComponent(color, 'b')};

    if(!(text_surface=TTF_RenderText_Solid(font, texto, fonteColor))) {
        printf("Erro TTF_RenderText_Solid");

    } else {

        apply_surface(x, y, text_surface, window_surface );

    }


}

string RoundDouble(double d, int precisao){

    std::string s;
    std::stringstream sstream;
    sstream.setf(std::ios::fixed);
    sstream.precision(precisao);
    sstream << d;

    return sstream.str();

}

void Grafico(){

    Uint32 corFundo = RGB(255, 255, 255);

    Uint32 corLinhas = RGB(0, 255, 0);
    Uint32 corEixo = RGB(0, 0, 255);

    //Desenha o fundo
    printRetangulo(graph_min, graph_max, true, corFundo);

    // imprime as linhas do gráfico
    for(int linha = coord_min.y; linha <= coord_max.y; linha+=1){
        LinhaCartesianaHorizontal(coord_min.x, coord_max.x, linha, corLinhas);
    }

    // imprime o eixo Y
    LinhaCartesianaHorizontal(coord_min.x, coord_max.x, 0, corEixo);


    // imprime as colunas do gráfico
    for(int coluna = coord_min.x; coluna <= coord_max.x; coluna+=1){
        string temp = std::to_string(coluna);
        LinhaCartesianaVertical(coord_min.y, coord_max.y, coluna, corLinhas);
    }

    // imprime o eixo X
    LinhaCartesianaVertical(coord_min.y, coord_max.y, 0, corEixo);


    int qtde_div = 10;
    int w = graph_max.x - graph_min.x;
    int h = graph_max.y - graph_min.y;

    double graph_div_x = (double)w/qtde_div;
    double graph_div_y = (double)h/qtde_div;

    Coordenada temp;
    string str_temp;
    Point p;
    int x,y;

    // imprime valores de X;
    for(int coluna = 0; coluna <= qtde_div; coluna+=1){
        p = getPoint(graph_min.x + coluna*graph_div_x, graph_max.y);

        temp = getCoordenadaFromPoint(p);

        for(int i = 0; i < 5; i++){
            setPixel(p.x, p.y, corLinhas);
            p.y++;
        }

        y++;
        PrintText(p.x, p.y, &RoundDouble(temp.x,2)[0] , corLinhas);

    }


    // imprime valores de Y;
    for(int linha = 0; linha <= qtde_div; linha+=1){
        p = getPoint(graph_min.x, graph_min.y + linha*graph_div_y);

        temp = getCoordenadaFromPoint(p);


        for(int i = 0; i < 5; i++){
            setPixel(p.x, p.y, corLinhas);
            p.x--;
        }

        p.x -= 40;
        p.y -= metadeSizeFonte;
        PrintText(p.x, p.y, &RoundDouble(temp.y,2)[0] , corLinhas);

    }


    //desenha novamente o retangulo para fazer uma borda, mas não pinta o interior
     printRetangulo(graph_min, graph_max, false, corLinhas);
}

void ImprimeFuncao(Funcao f){

    //std::cout << f.espressao << '\n';

    preparaFuncao(f.espressao);

    for (double x=coord_min.x; x < coord_max.x; x += delta)
    {
        double y;
        x_variable = x;
        y = te_eval(expr);

        DesenhaPontoMapeado(getCoordenada(x, y), f.color);
        //printf("Ponto(%f,%f)\n", x,y);
    }

}


void ImprimeCursor(){

    int sizeCursor = 6;
    int margem = 10;

    Uint32 cor = RGB(255,0,0);

    printBresenhamCircle(cursor.x, cursor.y, sizeCursor, cor );

    Coordenada coord_atual = getCoordenadaFromPoint(cursor);
    string temp = "(" + std::to_string(coord_atual.x) + ", " + std::to_string(coord_atual.y) + ")";
    PrintText(cursor.x + sizeCursor + margem , cursor.y - metadeSizeFonte, &temp[0] , cor);

}

void ImprimeFuncoes(){

    PrintText(5,0, "Funcoes:", RGB(255,255,255));

    int linha = 1;
    for (Funcao f : funcoes) {
        ImprimeFuncao(f);
        PrintText(15, linha * sizeFonte, &f.espressao[0], f.color);
        linha++;
	}

}


// Aqui ocorrem as chamadas das funções a ser exibidas na janela
void display(int i)
{
    Grafico();

    ImprimeFuncoes();

    ImprimeCursor();


}


// Mostra na barra de título da janela a posição
// corrente do mouse
void showMousePosition(SDL_Window * window, int x, int y)
{
    std::stringstream ss;

    if(x < graph_min.x){
        x = graph_min.x;
    }else if(x > graph_max.x){
        x = graph_max.x;
    }

    if(y < graph_min.y){
        y = graph_min.y;
    }else if(y > graph_max.y){
        y = graph_max.y;
    }

    cursor = getPoint(x,y);

    ss << titulo << " Ponto: (" << cursor.x << ", " << cursor.y << ")";
    SDL_SetWindowTitle(window, ss.str().c_str());
}


int CarregaFontes(){

    if (TTF_Init() < 0) {
        fprintf(stderr, "Couldn't initialize TTF: %s\n",SDL_GetError());
        SDL_Quit();
        return(-1);
    }

    font = TTF_OpenFont(nomeFonte, sizeFonte);
    if (font == NULL) {
        fprintf(stderr, "Couldn't load %d pt font from %s: %s\n", sizeFonte, nomeFonte, SDL_GetError());
        return(-1);
    }

}

// Inicializa o SDL, abre a janela e controla o loop
// principal do controle de eventos
int main()
{

    // Inicializações iniciais obrigatórias

    setlocale(LC_ALL, NULL);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow(titulo.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        SDL_WINDOW_RESIZABLE);

    window_surface = SDL_GetWindowSurface(window);

    pixels = (unsigned int *) window_surface->pixels;
    width = window_surface->w;
    height = window_surface->h;

    // Fim das inicializações

    printf("Pixel format: %s\n",
        SDL_GetPixelFormatName(window_surface->format->format));


    CarregaFontes();


    int i = 0;
     while (1)
    {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }


            switch( event.type ){
            /* Look for a keypress */
                case SDL_KEYDOWN:
                    /* Check the SDLKey values and move change the coords */
                    switch( event.key.keysym.sym ){
                        case SDLK_LEFT:
                            //printf("Pressionada a tecla LEFT\n");

                            coord_min.x -=taxaMudanca;
                            coord_max.x -=taxaMudanca;

                            //printf("Min: (%f,%f), Max: (%f,%f)\n", coord_min.x, coord_min.y, coord_max.x, coord_max.y);

                            break;
                        case SDLK_RIGHT:

                            //printf("Pressionada a tecla RIGHT\n");

                            coord_min.x +=taxaMudanca;
                            coord_max.x +=taxaMudanca;

                            //printf("Min: (%f,%f), Max: (%f,%f)\n", coord_min.x, coord_min.y, coord_max.x, coord_max.y);

                            break;
                        case SDLK_UP:

                            //printf("Pressionada a tecla UP\n");

                            coord_min.y +=taxaMudanca;
                            coord_max.y +=taxaMudanca;

                            //printf("Min: (%f,%f), Max: (%f,%f)\n", coord_min.x, coord_min.y, coord_max.x, coord_max.y);
                            break;
                        case SDLK_DOWN:

                            //printf("Pressionada a tecla DOWN\n");

                            coord_min.y -=taxaMudanca;
                            coord_max.y -=taxaMudanca;

                            //printf("Min: (%f,%f), Max: (%f,%f)\n", coord_min.x, coord_min.y, coord_max.x, coord_max.y);

                            break;

                        case SDLK_KP_PLUS:

                            //printf("Pressionada a tecla +\n");

                            if(coord_max.x - coord_min.x < 50 && coord_max.y - coord_min.y < 50 ){

                               coord_min.x -=taxaMudanca;
                                coord_min.y -=taxaMudanca;

                                coord_max.x +=taxaMudanca;
                                coord_max.y +=taxaMudanca;

                                delta = delta * (1 + taxaMudanca*0.2); // quando aumenta o gráfico aumenta o delta
                                //printf("Min: (%f,%f), Max: (%f,%f)\n", coord_min.x, coord_min.y, coord_max.x, coord_max.y);


                            }else{
                                printf("Nao eh permitido aumentar mais!\n");
                            }




                            break;

                        case SDLK_KP_MINUS:

                            //printf("Pressionada a tecla -\n");

                            // não permite diminuir muito
                            if(coord_max.x - coord_min.x > 4 && coord_max.y - coord_min.y > 4 ){
                                coord_min.x +=taxaMudanca;
                                coord_min.y +=taxaMudanca;

                                coord_max.x -=taxaMudanca;
                                coord_max.y -=taxaMudanca;

                                delta = delta / (1 + taxaMudanca*0.2); // quando diminui o gráfico diminui o delta

                                //printf("Min: (%f,%f), Max: (%f,%f)\n", coord_min.x, coord_min.y, coord_max.x, coord_max.y);

                            }else{
                                printf("Nao eh permitido diminuir mais!\n");
                            }



                            break;

                        case SDLK_RETURN:

                            Uint32 cores[5];
                            cores[0] = RGB(255,0,0);
                            cores[1] = RGB(0,255,255);
                            cores[2] = RGB(255,0,255);
                            cores[3] = RGB(255,255,0);
                            cores[4] = RGB(0,0,0);

                            //funcoes.push_back(getFuncao(obtemFuncao(), cores[NumeroRandomico(0,6)]));
                            funcoes.push_back(getFuncao(obtemFuncao(), cores[0]));

                            break;
                        default:
                            break;
                    }
                break;

            }


            //Coordenada coord_min = getCoordenada(-10,-10);
            //Coordenada coord_max = getCoordenada(10,10);

            //pos_x += xvel;
            //pos_y += yvel;


            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    window_surface = SDL_GetWindowSurface(window);
                    pixels = (unsigned int *) window_surface->pixels;
                    width = window_surface->w;
                    height = window_surface->h;
                    printf("Size changed: %d, %d\n", width, height);
                }
            }

            // Se o mouse é movimentado
            if(event.type == SDL_MOUSEMOTION)
            {
                // Mostra as posições x e y do mouse
                showMousePosition(window,event.motion.x,event.motion.y);
            }
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
				/*Se o botão esquerdo do mouse é pressionado */
                if(event.button.button == SDL_BUTTON_LEFT)
				{
					printf("Mouse pressed on (%d,%d)\n",event.motion.x,event.motion.y) ;
				}
            }
        }

        // Seta a cor de fundo da janela para a informada nas
        // constantes VERMELHO, VERDE e AZUL
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                setPixel(x, y, VERMELHO, VERDE, AZUL);
            }
        }

        i++;
        if(i == 101){
            i = 0;
        }
        display(i);

        SDL_UpdateWindowSurface(window);
    }
}


