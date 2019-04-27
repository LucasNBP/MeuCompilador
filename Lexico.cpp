#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

#define Tam_Maximo 524288
#define Tam_Max_ID 512
#define Tam_MaxReservedWords 8

struct Tokens{
	string tokenValue;
	string tokenName;
	bool reservedWord=false;
};

//estrutura para receber o retorno informado pelos automatos
struct valoresRetorno{
	int i;
	int contadorColuna;
	int contadorLinha;
	Tokens tokenGerado;
};

/*
Reserved words:
	$ATEH $BIT $DE $ENQUANTO $ESCREVA $FIM $FUNCAO $INICIO $INTEIRO $LEIA $NULO $PARA
	$PARE $REAL $RECEBA $SE $SENAO $VAR $VET $. $: $; $<- $+ $- $* $/ $% $** $( $) $[ $] $< $>
	$<= $>= $= $<> $" $& $| $!
*/

//automato para trabalhar iniciando com <
valoresRetorno gerarTokenMenor(char leitura[Tam_Maximo], int i, int contadorColuna, int contadorLinha){
	valoresRetorno retornos;
	Tokens tokenGerado;
	string acumularToken="";
	acumularToken+=leitura[i];

	i++;
	contadorColuna++;
	switch(leitura[i])
	{
		case '=':{ //menor igual
			acumularToken+=leitura[i];
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName="<=";
		}break;

		case '>':{ //diferente
			acumularToken+=leitura[i];
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName="<>";
		}break;

		case '-':{ //atribuicao
			acumularToken+=leitura[i];
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName="<-";
		}break;

		default :{ //menor p/ qualquer outro char
			i--;
			contadorColuna--;
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName="<";
		}break;
	}

	retornos.i=i;
	retornos.contadorColuna=contadorColuna;
	retornos.contadorLinha=contadorLinha;
	retornos.tokenGerado=tokenGerado;

	return retornos;
}

//automato para trabalhar iniciando com >
valoresRetorno gerarTokenMaior(char leitura[Tam_Maximo], int i, int contadorColuna, int contadorLinha){
	valoresRetorno retornos;
	Tokens tokenGerado;
	string acumularToken="";
	acumularToken+=leitura[i];

	i++;
	contadorColuna++;
	switch(leitura[i])
	{
		case '=':{ //maior igual
			acumularToken+=leitura[i];
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName=">=";
			break;
		}break;

		default :{ //maior p/ qualquer outro char
			i--;
			contadorColuna--;
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName=">";
		}break;
	}

	retornos.i=i;
	retornos.contadorColuna=contadorColuna;
	retornos.contadorLinha=contadorLinha;
	retornos.tokenGerado=tokenGerado;

	return retornos;
}

//automato para trabalhar iniciando com *
valoresRetorno gerarTokenMulExpo(char leitura[Tam_Maximo], int i, int contadorColuna, int contadorLinha){
	valoresRetorno retornos;
	Tokens tokenGerado;
	string acumularToken="";
	acumularToken+=leitura[i];

	i++;
	contadorColuna++;
	switch(leitura[i])
	{
		case '*':{ //expoente
			acumularToken+=leitura[i];
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName="**";
			break;
		}break;

		default :{ //* p/ qualquer outro char
			i--;
			contadorColuna--;
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName="*";
		}break;
	}

	retornos.i=i;
	retornos.contadorColuna=contadorColuna;
	retornos.contadorLinha=contadorLinha;
	retornos.tokenGerado=tokenGerado;

	return retornos;
}

//automato para trabalhar ID's e palavras reservadas
valoresRetorno gerarTokenIDR(char leitura[Tam_Maximo], int i, int contadorColuna, int contadorLinha){
	valoresRetorno retornos;
	Tokens tokenGerado;
	string acumularToken="";
	acumularToken+=leitura[i];

	i++;
	contadorColuna++;
	switch(leitura[i])
	{
		case '*':{ //expoente
			acumularToken+=leitura[i];
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName="**";
			break;
		}break;

		default :{ //* p/ qualquer outro char
			i--;
			contadorColuna--;
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName="*";
		}break;
	}

	retornos.i=i;
	retornos.contadorColuna=contadorColuna;
	retornos.contadorLinha=contadorLinha;
	retornos.tokenGerado=tokenGerado;

	return retornos;
}

//estrutura principal
int main(){
	ios_base::sync_with_stdio(false);

	char leitura[Tam_Maximo]; //variavel que recebera o codigo
	int tamanhoLeitura; //variavel com o tamanho da entrada dada
	bool flagParar=false; //flag para saber se continuo as analises ou nao
	int contadorColuna=0;
	int contadorLinha=1;
	string acumularToken="";
	Tokens tokenGerado; //variavel para juntar a leitura em um token
	valoresRetorno retornos; //variavel para receber o retorno informado pelos automatos
	vector<Tokens> listaTokens; //lista de tokens gerados
	tamanhoLeitura = fread(leitura, sizeof(char), Tam_Maximo, stdin);
	
	//conferindo se todos os caracteres pertencem as especificacoes
	for(int i=0; i<tamanhoLeitura; i++){
		if(leitura[i]>126 || leitura[i]<9 || (leitura[i]>10 && leitura[i]<32)){
			cout<<"ARQUIVO INVALIDO\n";
			flagParar=true;
			break;
		}
	}
	
	if(flagParar){
		return 0;
	}
	
	//analise lexica
	for(int i=0; i<tamanhoLeitura; i++){
		contadorColuna++;
		switch (leitura[i])
		{
			//espaco
			case 32:{
				contadorColuna++;
			}break;
			
			//quebra de linha
			case 10:{
				contadorLinha++;
				contadorColuna=0;
			}break;

			case('='): case('.'): case(':'): case(';'): case('+'): case('-'): case('/'): case('%'): 
			case('('): case(')'): case('['): case(']'): case('&'): case('|'): case('!'):{
				tokenGerado.tokenName=leitura[i];
				tokenGerado.tokenValue=leitura[i];
				tokenGerado.reservedWord=true;
				listaTokens.push_back(tokenGerado);
			}break;
			
			case '*':{
				retornos=gerarTokenMulExpo(leitura, i, contadorColuna, contadorLinha);
				i=retornos.i;
				contadorLinha=retornos.contadorLinha;
				contadorColuna=retornos.contadorColuna;
				tokenGerado=retornos.tokenGerado;
				tokenGerado.reservedWord=true;
				listaTokens.push_back(tokenGerado);
			}break;

			case '<':{
				retornos=gerarTokenMenor(leitura, i, contadorColuna, contadorLinha);
				i=retornos.i;
				contadorLinha=retornos.contadorLinha;
				contadorColuna=retornos.contadorColuna;
				tokenGerado=retornos.tokenGerado;
				tokenGerado.reservedWord=true;
				listaTokens.push_back(tokenGerado);
			}break;

			case '>':{
				retornos=gerarTokenMaior(leitura, i, contadorColuna, contadorLinha);
				i=retornos.i;
				contadorLinha=retornos.contadorLinha;
				contadorColuna=retornos.contadorColuna;
				tokenGerado=retornos.tokenGerado;
				tokenGerado.reservedWord=true;
				listaTokens.push_back(tokenGerado);
			}break;
			
			default :{
				if((leitura[i]>64 && leitura[i]<91) || (leitura[i]>96 && leitura[i]<123)){
					cout<<"\n viva?"<<endl;
				}
			}break;
		}
	}

	for(int i=0; i<listaTokens.size(); i++){
		cout<<"\n"<<listaTokens[i].tokenName<<"\n";
	}
	
	return 0;
}
