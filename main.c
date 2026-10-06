/*
Sistema: Jogo da velha em C
Desenvolvido por Richard henrique vulgo (livedriven)
Data: 05/10/2026
Versão: 1.0

Descrição:
Jogo da velha (3x3) em modo texto, no qual o jogador enfrenta o sistema
(computador) diretamente no terminal.

Funcionamento:
- O jogador informa um nick de 1 a 5 caracteres e escolhe jogar com X ou O;
  o sistema fica automaticamente com o símbolo restante.
- Um sorteio aleatório define quem faz a primeira jogada.
- O jogador marca sua jogada informando a linha e a coluna (de 1 a 3). O
  programa valida se a posição existe e se ainda está livre, pedindo uma
  nova entrada em caso de erro.
- O sistema joga seguindo uma ordem de prioridade:
    1. Se puder vencer na jogada, ele vence;
    2. Se o jogador estiver prestes a vencer, ele bloqueia;
    3. Caso contrário, ocupa a primeira posição livre na ordem:
       centro, cantos e, por último, laterais.
- A cada jogada o tabuleiro é exibido na tela.
- A partida termina quando alguém completa uma linha, coluna ou diagonal
  (vitória) ou quando o tabuleiro fica cheio sem vencedor ("Deu Velha!").
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

//Função auxiliar para exibir o tabuleiro
void exibirTabuleiro(char tabuleiro[3][3]){
    printf("\n ");  // espaço da coluna reservada para o número da linha

    for(int i = 0; i < 3; i ++){
        printf(" %d ", i +1); //Imprime o número da coluna
    }

    for(int i = 0; i < 3; i++){
        printf("\n%d", i+1); //Imprime o número da linha

        for(int j = 0; j < 3; j++){
            printf("[%c]",tabuleiro[i][j]);
        }
    }
}

//Função auxiliar para descartar tudo que sobrou no buffer de entrada
void limparBuffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

bool tabuleiroCheio(char t[3][3]){
    //Loop para percorrer todo o tabuleiro
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j ++){
            if(t[i][j] == ' '){
               return false; // Caso tenho 1 espaço em branco que seja, o tabuleiro ainda não estara cheio
               //e a função retornara false
            }
        }
    }

    return true; //Caso não tenha nenhum espaço a função retornara true
}

//Função auxiliar para sortea quem sera o primeiro a jogar
int sorteio (){
    return 1 + (rand() % 2); //Retorna um número aleatório (sendo eles só 1 ou 2)
}

// Retorna true se o símbolo recebido completou uma linha, coluna ou diagonal
bool ganhou(char t[3][3], char symbol){

    for(int i = 0; i < 3; i ++){
        //verificando as linhas i
        if(t[i][0] == symbol && t[i][0] ==  t[i][1] && t[i][1] == t[i][2]) return true;

        //verificando as colunas i
        if(t[0][i] == symbol && t[0][i] == t[1][i] && t[1][i] == t[2][i]) return true;
    }

    //verificando se existe uma lina completa nas diagonais
    return t[1][1] == symbol &&
        ((t[0][0] == t[1][1] && t[1][1] == t[2][2]) ||
         (t[0][2] == t[1][1] && t[1][1] == t[2][0]));
}

//Função responsavel por fazer simulações retornar se achou um resultado vencedor ou não
bool buscarJogadaVencedora(char t[3][3], char symbol, int *linha, int *coluna){
    for(int i = 0; i < 3; i ++){
        for(int j = 0; j < 3; j ++){
            //Verificando se a posição atual está marcada ou não
            if(t[i][j] != ' '){
                continue;//Caso esteja marcada pulamos esse loop e partimos para o proximo
            }

            t[i][j] = symbol; //simula uma jogada
            bool venceu = ganhou(t,symbol);
            t[i][j] = ' '; //desfaz a simulação

            //Verifica se alguma simulação chegou a vençer
            if(venceu){
                //Caso alguma simulação vença a função passa as coordenadas para
                //As variaveis de linha e coluna que serão usadas fora do escopo dessa função
                *linha = i;
                *coluna = j;
                return true; // e retorna true para verificações
            }
        }
    }
    return false; //Caso nenhuma simulação vença a função retorna false
}

//Função auxiliar para validar a existencia de uma posição no tabuleiro 3x3
bool posicaoValida(int linha, int coluna){
    //Variaveis que guardam o tamanho da matriz (que é estatico)
    int linhas = 3;
    int colunas = 3;

    //Verifica se as coordenadas passadas existem dentro da matriz
    if((linha >= 0 && linha < linhas)&& (coluna >= 0 && coluna < colunas)){
        //Caso exista retorna true
        return true;
    }else{
        //Senão retorna false
        return false;
    }
}

//Função responsavel por validar a escolhar do jogador e automaticamente definir o simbolo do sistema
void choice(char *jogador, char *sistema){
    do{
        printf("\nJogador escolha o simbolo que ira jogar X / O\n");
        scanf(" %c", jogador);
        limparBuffer(); //Descarta o que sobrou (ex: o "O" de "XO")

        *jogador = toupper(*jogador);

    } while(*jogador != 'X' && *jogador != 'O');

    //Switch case para verificar a escolha do jogador e atribuir ao sistema o simbolo que restou
    switch(*jogador){
        case 'X':
            *sistema = 'O';
            break;

        case 'O':
            *sistema = 'X';
            break;
    }
}

//Função auxiliar para exibir os simbolos dos jogadores
void renderChoice(char nickName[6], char jogador, char sistema){
    printf("O jogador: %s\n", nickName);
    printf("Escolheu: %c\n", jogador);
    printf("O sistema ficou com: %c\n", sistema);
}

//Função responsavel por armazenar na memoria o nome do jogador
void definirNome(char *nome){
    char buffer[100];

    while(true){
        printf("Digite seu nick (no máximo 5 letras):\n");
        fgets(buffer,sizeof(buffer),stdin);

        buffer[strcspn(buffer, "\n")] = '\0'; //Normalizando entrada do jogador

        //Velidando se o que o jogador digitou e valido
        if(strlen(buffer) == 0 || strlen(buffer) > 5){
            //Se a validação não passar o sistema imprime uma mensagem de alerta e continua o loop
            printf("Nick inválido! Use de 1 a 5 caracteres.\n");
        }else{
            //Se a validação passar
            //iremos copiar o conteudo digitado para a variavel responsavel por armazenar o nome do jogador
            strcpy(nome,buffer);
            break;//quebramos o loop caso a validação passe
        }

    }

    //Após todas as validações passarem impimimos uma mensagem de boas vindas ao jogador
    printf("Olá seja bem vindo ao jogo da velha %s\n", nome);
}

//Função responsavel pelas jogadas do sistema
void jogadaSistema(char t[3][3],char symbolPlayer, char symbolSystem){
    //Variaveis que armazenam a coordanadas da jogada que o sistema irá fazer
    int linha = -1;
    int coluna = -1;

    //Verificando se não existe nenhuma possibilidade de ganhar e se não existe risco do jogador ganhar
    if(!buscarJogadaVencedora(t,symbolSystem,&linha,&coluna) && !buscarJogadaVencedora(t,symbolPlayer,&linha,&coluna)){
        //Lista das coordenadas dos quadrados que o sistema deve focar em marca
        int prioridade[9][2] = {
            {1,1},{0,0},{0,2},{2,0},{2,2},{0,1},{1,0},{2,1},{1,2}
        };

        //Loop para verificar quais quadrados estão limpos
        for(int i = 0; i < 9; i ++){
            if(t[prioridade[i][0]][prioridade[i][1]] == ' '){
                //Caso um quadrado prioritario esteja limpo o sistema atribui as coordenadas a essa posição e encerra o loop
                linha = prioridade[i][0];
                coluna = prioridade[i][1];
                break;
            }
        }
    }

    //Caso o sistema tenha uma jogada ganhadora ou tenha que impedir o jogador de ganhar
    //pulamos para esse trecho do código e o sistema faz a jogada dele.
    //Caso não aconteca nenhuma das duas possibilidades o sistema passa pela verificação das posições prioritarias e após escolher a primeira limpa
    //o sistema vem para esse trecho e faz a jogada
    t[linha][coluna] = symbolSystem;
}


void marcarTabuleiro(char tabuleiro[3][3], char symbol){
    while(true){
        //Variaveis para armazenar temporariamente a escolha do jogador
        int linha;
        int coluna;

        printf("\nDigite linha da onde você quer marcar\n");
        if(scanf("%d", &linha) != 1){
            //Se não for número, limpa o buffer e pede novamente
            limparBuffer();
            printf("Digite apenas números!\n");
            continue;
        }
        limparBuffer();

        printf("\nDigite a coluna da onde você quer marcar\n");
        if(scanf("%d", &coluna) != 1){
            limparBuffer();
            printf("Digite apenas números!\n");
            continue;
        }
        limparBuffer();

        //Verificando se a coordenada existe na matriz
        if(!posicaoValida(linha - 1, coluna - 1)){
            printf("A posição escolhida e invalida!!\n");
            continue;
        }
        //Verifica se o quadrado já tem marcação
        else if(tabuleiro[linha - 1][coluna - 1] != ' '){
            printf("A posição escolhida já foi marcada, escolha outra\n");
            continue;
        }
        else{
            //Caso nenhuma das outras verificações seja true a jogada é efetuada
            printf("\nMarcação feita com sucesso\n");
            tabuleiro[linha - 1][coluna - 1] = symbol;
            exibirTabuleiro(tabuleiro);
            break;
        }
    }
}

int main(){
    //Inicializando a semente usando o tempo atual do sistema para que o sorteio seja realmente aleatório
    srand(time(NULL));

    //Inicializando a matriz
    char tabuleiro[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}
    };

    char nickName[6];//Variavel para armazenar o nome/nick do jogador

    char symbolPlayer;//Variavel para armazena o simbolo do jogador
    char symbolSystem;//Simbolo do sistema

    definirNome(nickName);//Mandando o jogador a definir o nome dele

    choice(&symbolPlayer, &symbolSystem);//Mandando o jogador definir o simbolo com o qual ele deseja jogar

    renderChoice(nickName,symbolPlayer,symbolSystem); //Exibindo as escolhas do jogador

    exibirTabuleiro(tabuleiro);

    int vez = sorteio();//sorteando quem ira começar
    //Jogador - 1 / Sistema - 2

    printf("\nO %s começa\n", vez == 1 ? nickName : "sistema");

    while(true){
        char atual;//Variavel que armazena o simbolo de quem está jogando no turno atual

        //Verificações de turno
        if(vez == 1){
            //Caso seja a vez do jogador, chamamos as funções necessarias para ele jogar
            marcarTabuleiro(tabuleiro,symbolPlayer);
            atual = symbolPlayer; //E armazenamos o simbolo dele na variavel temporaria
        }else{
            //Caso seja o turno do sistema chamamos as funções necessarias para ele jogar
            jogadaSistema(tabuleiro,symbolPlayer,symbolSystem);
            exibirTabuleiro(tabuleiro);
            atual = symbolSystem;//E armazenamos o simbolo dele na variavel temporaria
        }

        //Verificando possiveis resultados do jogo
        if(ganhou(tabuleiro,atual)){
            printf("\nO %s ganhou", vez == 1 ? nickName : "Sistema"); //Imprime mensagem de vitoria para quem ganhou
            break;//Finaliza o loop aqui
        }

        //Verificando se deu velha
        if(tabuleiroCheio(tabuleiro)){
            printf("\nDeu Velha!");//Imprime mensagem de que deu Velha
            break;//Finaliza o loop
        }

        //Caso nenhum resultado tenha acontecido ainda, o jogo segue para o proximo turno
        vez = vez == 1 ? 2 : 1;
    }



    return 0;
}
