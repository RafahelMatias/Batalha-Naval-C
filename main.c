#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netdb.h>

#define CLOSESOCKET close
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKADDR_IN struct sockaddr_in
#define SOCKLEN_T socklen_t
#define INET_PTON inet_pton
#define PORTA2 4950
#define PORTA1 4960
#define MAXBUFLEN 100
#define TAM 26


// Esse código representa um jogo de batalha naval com um modo automatico e
//    conectividade entre computadores para que dois players possam jogar de computadores diferentes



void iniciar_planetas(const char *nome_arquivo, char planeta_jogador[TAM][TAM], char planeta_inimigo[TAM][TAM]) {
    //planeta do inimigo
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            planeta_inimigo[i][j] = '-';
        }
    }

    //planeta do jogador
    FILE *arq = fopen(nome_arquivo, "rt");
    if (!arq) 
    {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    char linha[100][100];

    while (!feof(arq))
    {
        fgets(linha[i], 100, arq);
     
        i++;
    }
    fclose(arq);

    for (int lin = 0; lin<26; lin++)
    {
        for (int col = 0; col < 100; col++) 
        {
            if (isalnum(linha[lin][col]))
            {
                planeta_jogador[lin][col/2] = linha[lin][col];
            }
            if (linha[lin][col] == '\n')
            {
                break;
            }
        }
    }

} // acaba a função iniciar_planetas



void exibir_baixas(int v_cont[], int cont)
{

    if (v_cont[11] == 0)
    {
        printf("Baixas aliadas: %i\n",v_cont[11]);
    }
    else if (v_cont[11] == 1)
    {
        printf("Baixas aliadas: %i Milhão\n",v_cont[11]);
    } 
    else printf("Baixas aliadas: %i Milhões\n",v_cont[11]);
    

    if (cont == 0)
    {
        printf("Baixas inimigas: %i\n",cont);
    }
    else if (cont == 1)
    {
        printf("Baixas inimigas: %i Milhão\n",cont);
    } 
    else printf("Baixas inimigas: %i Milhões\n",cont);

    printf("\n");

} // acaba a função exibir_baixas






void exibir_planetas(char planeta_jogador[TAM][TAM], char planeta_inimigo[TAM][TAM], int v_cont[], int cont)
{
    printf("\nPlaneta do Jogador:\n");
    for (int i = 0; i < 26; i++)
    {
        if (i==0){printf("  ");}
        printf("%i ",i);
        if (i<10)
        {
            printf(" ");
        }
        
    }
    printf("\n");

    for (int i = 0; i < 26; i++)
    {
        printf("%c ", 'A' + i);

        for (int j = 0; j < 26; j++)
        {
            if (planeta_jogador[i][j] == '0') // se for Mar
            {
                printf("-  ");
            }
            else
            printf("%c  ", planeta_jogador[i][j]);
            
        }
        printf("\n");
    }

    // ---------------------

    printf("\nPlaneta do Inimigo:\n");
    for (int i = 0; i < 26; i++)
    {
        if (i==0){printf("  ");}
        printf("%i ",i);
        if (i<10)
        {
            printf(" ");
        }
        
    }
    printf("\n");

    for (int i = 0; i < 26; i++)
    {
        printf("%c ", 'A' + i);

        for (int j = 0; j < 26; j++)
        {
            if (planeta_inimigo[i][j] == '0') // se for Mar
            {
                printf("-  ");
            }
            else
            printf("%c  ", planeta_inimigo[i][j]);
            
        }
        printf("\n");
    }
    printf("\n");

    exibir_baixas(v_cont, cont);

}// acabou o exibir planetas





void enviar_dados(const char *ip, const char *mensagem, int porta) 
{
    int sockfd;
    struct sockaddr_in seu_endereco; // informação do endereço do conector
    struct hostent *he;
    int numbytes;

    
    if ((he = gethostbyname(ip)) == NULL) {  // get the host info
        herror("gethostbyname");
        exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    seu_endereco.sin_family = AF_INET;     
    seu_endereco.sin_port = htons(porta); 
    seu_endereco.sin_addr = *((struct in_addr *)he->h_addr);
    memset(seu_endereco.sin_zero, '\0', sizeof seu_endereco.sin_zero);


    if ((numbytes = sendto(sockfd, mensagem, strlen(mensagem), 0,
                (struct sockaddr *)&seu_endereco, sizeof seu_endereco)) == -1)
    {
        perror("sendto");
        printf("Entrou no erro do enviar dados, antes do close\n");
        close(sockfd);
        printf("Entrou no erro do enviar dados\n");
        exit(1);
    }

    inet_ntoa(seu_endereco.sin_addr);


    close(sockfd);

} // acaba a função envia_dados




void receber_dados(char *buf, int porta) 
{
    int sockfd;
    struct sockaddr_in meu_endereco;    // a informação de meu endereço
    struct sockaddr_in seu_endereco; // Informação do endereço de quem se conecta
    socklen_t tam_endereco;
    int numbytes;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    meu_endereco.sin_family = AF_INET;        
    meu_endereco.sin_port = htons(porta);     
    meu_endereco.sin_addr.s_addr = INADDR_ANY; 
    memset(meu_endereco.sin_zero, '\0', sizeof meu_endereco.sin_zero);

    if (bind(sockfd, (struct sockaddr *)&meu_endereco, sizeof meu_endereco) == -1) {
        perror("bind");
        close(sockfd);  // Adiciona close(sockfd) aqui para fechar o socket em caso de erro
        exit(1);
    }

    tam_endereco = sizeof seu_endereco;


        if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1, 0,
            (struct sockaddr *)&seu_endereco, &tam_endereco)) == -1) {
            perror("recvfrom");
            close(sockfd);  // Adiciona close(sockfd) aqui para fechar o socket em caso de erro
            exit(1);
        }

        
        buf[numbytes] = '\0';
        

    close(sockfd);

} // acaba a função receber_dados






void recebeu_tiro(const char *tiro, char *resposta, char planeta_jogador[TAM][TAM], char planeta_inimigo[TAM][TAM], int v_cont[])
{

    int coluna = tiro[1] - '0';
    int linha = tiro[0] - 'A';

    if (tiro[2] != '\0')
    {
        coluna = (coluna * 10) + tiro[2] - '0';
    }

    char loc_tiro = planeta_jogador[linha][coluna];


    if (loc_tiro == '0')
    {
        sprintf(resposta, "X"); // Mar
        planeta_jogador[linha][coluna] = 'X';
        printf("x\n");
    } 
    else 
    {
        if (loc_tiro == 'a' || loc_tiro == 'b' || loc_tiro == 'c' || loc_tiro == 'd') // ponto
        {
            v_cont[0] = v_cont[0] +1;
            v_cont[11] = v_cont[11] +1;
            sprintf(resposta, "Z.");
            planeta_jogador[linha][coluna] = 'Y';
        }
        if (loc_tiro == 'e') // ipequeno
        {
            v_cont[1] = v_cont[1] +1;
            v_cont[11] = v_cont[11] +1;
            if (v_cont[1] < 2)
            {
                sprintf(resposta, "Y"); 
            }
            else
            {
                sprintf(resposta, "Zi"); 
            } 
            planeta_jogador[linha][coluna] = 'Y';
        }
        if (loc_tiro == 'f') // ipequeno
        {
            v_cont[2] = v_cont[2] +1;
            v_cont[11] = v_cont[11] +1;
            if (v_cont[2] < 2)
            {
                sprintf(resposta, "Y"); 
            }
            else
            {
                sprintf(resposta, "Zi"); 
            } 
            planeta_jogador[linha][coluna] = 'Y';
        }
        if (loc_tiro == 'g') // Imaior
        {
            v_cont[3] = v_cont[3] +1;
            v_cont[11] = v_cont[11] +1;
            if (v_cont[3] < 4)
            {
                sprintf(resposta, "Y");
            }
            else
            {
                sprintf(resposta, "ZI");
            } 
            planeta_jogador[linha][coluna] = 'Y';
        }
        if (loc_tiro == 'h') // Imaior
        {
            v_cont[4] = v_cont[4] +1;
            v_cont[11] = v_cont[11] +1;
            if (v_cont[4] < 4)
            {
                sprintf(resposta, "Y");
            }
            else
            {
                sprintf(resposta, "ZI");
            } 
            planeta_jogador[linha][coluna] = 'Y';
        }
        if (loc_tiro == 'i') // principal
        {
            v_cont[5] = v_cont[5] +1;
            v_cont[11] = v_cont[11] +1;
            if (v_cont[5] < 5)
            {
                sprintf(resposta, "Y"); 
            }
            else
            {
                sprintf(resposta, "Zp");
            } 
            planeta_jogador[linha][coluna] = 'Y';
        }
        if (loc_tiro == 'j') // quadrada
        {
            v_cont[6] = v_cont[6] +1;
            v_cont[11] = v_cont[11] +1;
            if (v_cont[6] < 4)
            {
                sprintf(resposta, "Y"); 
            }
            else
            {
                sprintf(resposta, "Zq");
            } 
            planeta_jogador[linha][coluna] = 'Y';
        }
        if (loc_tiro == 'k') // quadrada
        {
            v_cont[7] = v_cont[7] +1;
            v_cont[11] = v_cont[11] +1;
            if (v_cont[7] < 4)
            {
                sprintf(resposta, "Y"); 
            }
            else
            {
                sprintf(resposta, "Zq");
            } 
            planeta_jogador[linha][coluna] = 'Y';
        }
        if (loc_tiro == 'l') // triangular
        {
            v_cont[8] = v_cont[8] +1;
            v_cont[11] = v_cont[11] +1;
            if (v_cont[8] < 3)
            {
                sprintf(resposta, "Y"); 
            }
            else
            {
                sprintf(resposta, "Zt");
            } 
            planeta_jogador[linha][coluna] = 'Y';
        }
        if (loc_tiro == 'm') // triangular
        {
            v_cont[9] = v_cont[9] +1;
            v_cont[11] = v_cont[11] +1;
            if (v_cont[9] < 3)
            {
                sprintf(resposta, "Y"); 
            }
            else
            {
                sprintf(resposta, "Zt");
            } 
            planeta_jogador[linha][coluna] = 'Y';
        }
        if (loc_tiro == 'n') // triangular
        {
            v_cont[10] = v_cont[10] +1;
            v_cont[11] = v_cont[11] +1;
            if (v_cont[10] < 3)
            {
                sprintf(resposta, "Y"); 
            }
            else
            {
                sprintf(resposta, "Zt");
            } 
            planeta_jogador[linha][coluna] = 'Y';
        }

    } // acaba o else

} // acaba a função recebeu_tiro




int verifica_acerto(const char *resposta)
{
    return !strstr(resposta, "X") != NULL;
}



int verifica_perdeu(int v_cont[]) // função que verifica se o jogador perdeu o jogo
{
    if (v_cont[11] == 38)
    {
        return 1;
    }
    else return 0;
}



int verifica_txt(char planeta_jogador[TAM][TAM]) // função que verifica se o arquivo txt esta feito da forma certa de acordo com o codigo
{
    int cont[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int cont_resp[] = {1,1,1,1,2,2,4,4,5,4,4,3,3,3};
    int i;
    for (int l=0; l<26; l++)
    {
        for (int c=0; c<26; c++)
        {
            if (planeta_jogador[l][c] == 'a')
            {
                i=0;
                cont[i] = cont[i] + 1;
            }
            if (planeta_jogador[l][c] == 'b')
            {
                i=1;
                cont[i] = cont[i] + 1;
            }
            if (planeta_jogador[l][c] == 'c')
            {
                i=2;
                cont[i] = cont[i] + 1;
            }
            if (planeta_jogador[l][c] == 'd')
            {
                i=3;
                cont[i] = cont[i] + 1;
            }
            if (planeta_jogador[l][c] == 'e')
            {
                i=4;
                cont[i] = cont[i] + 1;
            }
            if (planeta_jogador[l][c] == 'f')
            {
                i=5;
                cont[i] = cont[i] + 1;
            }
            if (planeta_jogador[l][c] == 'g')
            {
                i=6;
                cont[i] = cont[i] + 1;
            }
            if (planeta_jogador[l][c] == 'h')
            {
                i=7;
                cont[i] = cont[i] + 1;
            }
            if (planeta_jogador[l][c] == 'i')
            {
                i=8;
                cont[i] = cont[i] + 1;
            }
            if (planeta_jogador[l][c] == 'j')
            {
                i=9;
                cont[i] = cont[i] + 1;
            }
            if (planeta_jogador[l][c] == 'k')
            {
                i=10;
                cont[i] = cont[i] + 1;
            }
            if (planeta_jogador[l][c] == 'l')
            {
                i=11;
                cont[i] = cont[i] + 1;
            }
            if (planeta_jogador[l][c] == 'm')
            {
                i=12;
                cont[i] = cont[i] + 1;
            }
            if (planeta_jogador[l][c] == 'n')
            {
                i=13;
                cont[i] = cont[i] + 1;
            }
        }
    } // acaba o for

    for (int i = 0; i<14; i++)
    {
        if (cont[i] != cont_resp[i])
        {
            return 0;
        }
    }
    return 1;

} // acaba a função varifica_txt





int batalha(const char *arq, const char *ip_oponente, int autom) // função do jogo
{
    char planeta_jogador[TAM][TAM];
    char planeta_inimigo[TAM][TAM];

    int v_cont[] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int cont=0;


    char buf[MAXBUFLEN];
    int vez_jogador;

    printf("\n ------------------------------------ REGRAS -------------------------------------\n|                                                                                 |\n|        Para a utilização da opção automatica,                                   |\n|          o jogador que não começar deverá responder a pergunta abaixo antes.    |\n|                                                                                 |\n|        X: errou                                                                 |\n|        Y: acertou uma ilha                                                      |\n|                                                                                 |\n|        Em caso de destruicao completa da ilha,                                  |\n|        as seguintes mensegens irao aparecer com suas respectivas ilhas:         |\n|        Z.: ilha ponto                                                           |\n|        Zi: ilha Ipequeno                                                        |\n|        ZI: ilha Imaior                                                          |\n|        Zp: ilha principal                                                       |\n|        Zq: ilha quadrada                                                        |\n|        Zt: ilha triangular                                                      |\n|                                                                                 |\n ---------------------------------------------------------------------------------\n");
   
    printf("\nVocê que vai começar? \nPara Sim -> 1\nPara Não -> 0\nResponda: ");
    scanf("%d", &vez_jogador);
    printf("\n");

    iniciar_planetas(arq, planeta_jogador, planeta_inimigo);

    if (!verifica_txt(planeta_jogador))
    {
        printf("\nO arquivo %s está incorreto.\n\nComo montar o planeta de forma correta:\n->\nAs letras a, b, c e d devem ser usadas para a ilha ponto\nAs letras e e f devem ser usadas para a ilha Ipequeno\nAs letras g e h devem ser usadas para a ilha Imaior\nAs letras i devem ser usadas para a ilha principal\nAs letras j e k devem ser usadas para a ilha quadrada\nAs letras l, m e n devem ser usadas para a ilha triangular\n", arq);
        return 0;
    }
    int col_a=0, lin_a=0;

    while (1) {
        exibir_planetas(planeta_jogador, planeta_inimigo, v_cont, cont);


        if (vez_jogador) //-----------------------------------
        {
            char tiro[10];
            while (1)
            {
                if (autom == 1) break;

                printf("Envie o seu tiro (por exemplo, D7 ou h21): ");
                scanf("%s", tiro);
                
                if (strlen(tiro) > 3 || strlen(tiro) < 2 )
                {
                    printf("Digitou errado\n\n");
                    continue;
                }
                if (!isalpha(tiro[0]) || !isdigit(tiro[1]))
                {
                    printf("Digitou errado\n\n");
                    continue;
                }
                if (strlen(tiro) == 3)
                {
                    if (!isdigit(tiro[2]))
                    {
                        printf("Digitou errado\n\n");
                        continue;
                    }
                }
                int col = tiro[1] - '0';
                if (strlen(tiro) == 3)
                {
                    col = ((col * 10) + (tiro[2] - '0'));
                }
                 
                if (col > 25)
                {
                    printf("Digitou errado\n\n");
                    continue;
                }
                
                tiro[0] = toupper(tiro[0]);
                int lin = tiro[0] - 'A';
                
                if (planeta_inimigo[lin][col] == 'X' || planeta_inimigo[lin][col] == 'Y') // tiro repetido
                {
                    printf("Você ja enviou esse tiro, envie outro.\n\n");
                    continue;
                }
                break;
            }

            if (autom) // automatico
            {
                if (lin_a == 26)
                {
                    lin_a = 0;
                    col_a++;
                }

                
                tiro[0] = 'A' + lin_a;
                if (col_a>=10)
                {
                    tiro[1] = '0' + col_a / 10;
                    tiro[2] = '0' + col_a % 10;
                    tiro[3] = '\0';
                }
                else
                {
                    tiro[1] = '0' + col_a;
                    tiro[2] = '\0';
                } 
                
                printf("Tiro escolhido no modo automatico: %s\n",tiro);
                sleep(1);
                lin_a++;
            }

            enviar_dados(ip_oponente, tiro, PORTA1);
            sleep(1);
            receber_dados(buf, PORTA2);

            printf("Resposta do oponente: %s\n", buf);

            int coluna = tiro[1] - '0';
            int linha = tiro[0] - 'A';

            if (tiro[2] != '\0')
            {
                coluna = (coluna * 10) + tiro[2] - '0';
            }

            if (verifica_acerto(buf)) 
            {
                printf("Você acertou!\n");
                cont++;
                planeta_inimigo[linha][coluna] = 'Y';
                if (cont == 38)
                {
                    exibir_planetas(planeta_jogador, planeta_inimigo, v_cont, cont);
                    printf("\nVocê ganhou o jogo!!!\n");
                    break;
                }
            } 
            else 
            {
                printf("Você errou. Aguarde o tiro do oponente.\n");
                planeta_inimigo[linha][coluna] = 'X';
                vez_jogador = 0;
            }
            
        } 
        
        
        else //----------  Vez do oponente ---------------------
        {
            printf("Aguardando o tiro do oponente...\n");
            receber_dados(buf, PORTA1);

            char resposta[20];
            recebeu_tiro(buf, resposta, planeta_jogador, planeta_inimigo, v_cont);

            sleep(2);
            enviar_dados(ip_oponente, resposta, PORTA2);

            printf("Tiro recebido: %s\n", buf);

            if (!verifica_acerto(resposta)) 
            {
                printf("O oponente errou. Sua vez de jogar.\n");
                vez_jogador = 1;
            }
            if (verifica_perdeu(v_cont))
                {
                    exibir_planetas(planeta_jogador, planeta_inimigo, v_cont, cont);
                    printf("\nVocê perdeu o jogo.\n");
                    break;
                }
        }

    } // acaba o while(1)

    return 0;

} // acaba a função batalha




int main(int argc, char *argv[]) // Portas usadas: 4950 e 4960
{
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Uso: %s <Planeta> <IP do oponente> <Opção de automatico (-a)> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int autom=0;
    const char *arq = argv[1];
    const char *ip = argv[2];

    if (argc == 4)
    {
        if (strcmp(argv[3],"-a")==0)
        {
            autom = 1;
        }
        else
        {
            fprintf(stderr, "Uso: %s <Planeta> <IP do oponente> <Opção de automatico (-a)> \n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    batalha(arq, ip, autom);

    return 0;
    }