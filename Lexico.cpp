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
	bool numReal=false;
};

//estrutura para receber o retorno informado pelos automatos
struct valoresRetorno{
	int i;
	int contadorColuna;
	int contadorLinha;
	Tokens tokenGerado;
	bool flagParar=false;
};

/*
Reserved words:
	$ATEH $BIT $DE $ENQUANTO $ESCREVA $FIM $FUNCAO $INICIO $INTEIRO $LEIA $NULO $PARA
	$PARE $REAL $RECEBA $SE $SENAO $VAR $VET $. $: $; $<- $+ $- $* $/ $% $** $( $) $[ $] $< $>
	$<= $>= $= $<> $" $& $| $!
*/

//funcao para imprimir erro linha e coluna
void imprimirErro(int contadorLinha, int contadorColuna){
	cout<<contadorLinha<<" "<<contadorColuna<<endl;
}

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
valoresRetorno gerarTokenIDR(char leitura[Tam_Maximo], int i, int contadorColuna, int contadorLinha, vector<string> listaPalavrasReservadas){
	valoresRetorno retornos;
	Tokens tokenGerado;
	string acumularToken="";
	acumularToken+=leitura[i];
	
	i++;
	contadorColuna++;
	while((47<leitura[i] && leitura[i]<58) || (64<leitura[i] && leitura[i]<91) || (96<leitura[i] && leitura[i]<123)){
		if(acumularToken.size()<Tam_Max_ID){
			acumularToken+=leitura[i];
			i++;
			contadorColuna++;
		}else{
			//escrever o erro aqui, na posicao anterior coluna--
			imprimirErro(contadorLinha, contadorColuna-1);
			retornos.flagParar=true;
			break;
		}
	}
	
	i--;
	contadorColuna--;
	tokenGerado.tokenValue=acumularToken;
	
	if(acumularToken.size()<=Tam_MaxReservedWords){
		for(int i=0; i<listaPalavrasReservadas.size(); i++){
			if(acumularToken==listaPalavrasReservadas[i]){
				tokenGerado.tokenName=acumularToken;
				tokenGerado.reservedWord=true;
				//cout<<"palavra reservada\n";
			}
		}
	}
	
	if(!tokenGerado.reservedWord){
		tokenGerado.tokenName="ID";
	}
			
	retornos.i=i;
	retornos.contadorColuna=contadorColuna;
	retornos.contadorLinha=contadorLinha;
	retornos.tokenGerado=tokenGerado;

	return retornos;
}

//automato para trabalhar com constantes numericas
valoresRetorno gerarTokenNum(char leitura[Tam_Maximo], int i, int contadorColuna, int contadorLinha){
	valoresRetorno retornos;
	Tokens tokenGerado;
	string acumularToken="";
	acumularToken+=leitura[i];
	bool virgula=false;
	
	i++;
	contadorColuna++;
	while((47<leitura[i] && leitura[i]<58) || leitura[i]==44){
		if(acumularToken.size()<Tam_Max_ID){
			if(virgula && leitura[i]==44){
				imprimirErro(contadorLinha, contadorColuna-1);
				retornos.flagParar=true;
				break;
			}	
			acumularToken+=leitura[i];
			if(leitura[i]==44){
				virgula=true;
				tokenGerado.numReal=true;
			}
			i++;
			contadorColuna++;
		}else{
			//escrever o erro aqui, na posicao anterior coluna--
			imprimirErro(contadorLinha, contadorColuna-1);
			retornos.flagParar=true;
			break;
		}
	}
	
	if(('a'<=leitura[i] && leitura[i]<='z') || ('A'<=leitura[i] && leitura[i]<='Z')){
		imprimirErro(contadorLinha, contadorColuna-1);
		retornos.flagParar=true;
	}
	
	i--;
	contadorColuna--;
	tokenGerado.tokenValue=acumularToken;
	tokenGerado.tokenName="NUM";
				
	retornos.i=i;
	retornos.contadorColuna=contadorColuna;
	retornos.contadorLinha=contadorLinha;
	retornos.tokenGerado=tokenGerado;

	return retornos;
}

//automato para trabalhar string comecadas em "
valoresRetorno gerarTokenString(char leitura[Tam_Maximo], int i, int contadorColuna, int contadorLinha){
	valoresRetorno retornos;
	Tokens tokenGerado;
	string acumularToken="";
	bool fecharAspas=false;
		
	i++;
	contadorColuna++;
	
	while(!fecharAspas){
		if(leitura[i]==34){
			fecharAspas=true;
			break;
		}else{
			acumularToken+=leitura[i];
			if(leitura[i]==10){
				contadorLinha++;
				contadorColuna=0;
			}
			i++;
			contadorColuna++;
		}
	}
	
	tokenGerado.tokenValue=acumularToken;
	tokenGerado.tokenName="STRING";
	
	retornos.i=i;
	retornos.contadorColuna=contadorColuna;
	retornos.contadorLinha=contadorLinha;
	retornos.tokenGerado=tokenGerado;
	
	/*while((47<leitura[i] && leitura[i]<58) || leitura[i]==44){
		if(acumularToken.size()<Tam_Max_ID){
			if(virgula && leitura[i]==44){
				imprimirErro(contadorLinha, contadorColuna-1);
				retornos.flagParar=true;
				break;
			}	
			acumularToken+=leitura[i];
			if(leitura[i]==44){
				virgula=true;
				tokenGerado.numReal=true;
			}
			i++;
			contadorColuna++;
		}else{
			//escrever o erro aqui, na posicao anterior coluna--
			imprimirErro(contadorLinha, contadorColuna-1);
			retornos.flagParar=true;
			break;
		}
	}
	
	if(('a'<=leitura[i] && leitura[i]<='z') || ('A'<=leitura[i] && leitura[i]<='Z')){
		imprimirErro(contadorLinha, contadorColuna-1);
		retornos.flagParar=true;
	}
	
	i--;
	contadorColuna--;
	tokenGerado.tokenValue=acumularToken;
	tokenGerado.tokenName="NUM";
				
	retornos.i=i;
	retornos.contadorColuna=contadorColuna;
	retornos.contadorLinha=contadorLinha;
	retornos.tokenGerado=tokenGerado;*/

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
	
	/*vector<string> listaPalavrasReservadas{"ATEH", "BIT", "DE", "ENQUANTO", "ESCREVA", "FIM", "FUNCAO", "INICIO", "INTEIRO", 
	                                       "LEIA", "NULO", "PARA", "PARE", "REAL", "RECEBA", "SE", "SENAO", "VAR", "VET"};
	vector<string> listaPalavrasReservadas;
	listaPalavrasReservadas.push_back("BIT");*/
	
	vector<string> listaPalavrasReservadas(19);
	listaPalavrasReservadas[0]=("BIT");
	listaPalavrasReservadas[1]=("ATEH");
	listaPalavrasReservadas[2]=("DE");
	listaPalavrasReservadas[3]=("ENQUANTO");
	listaPalavrasReservadas[4]=("ESCREVA");
	listaPalavrasReservadas[5]=("FIM");
	listaPalavrasReservadas[6]=("FUNCAO");
	listaPalavrasReservadas[7]=("INICIO");
	listaPalavrasReservadas[8]=("INTEIRO");
	listaPalavrasReservadas[9]=("LEIA");
	listaPalavrasReservadas[10]=("NULO");
	listaPalavrasReservadas[11]=("PARA");
	listaPalavrasReservadas[12]=("PARE");
	listaPalavrasReservadas[13]=("REAL");
	listaPalavrasReservadas[14]=("RECEBA");
	listaPalavrasReservadas[15]=("SE");
	listaPalavrasReservadas[16]=("SENAO");
	listaPalavrasReservadas[17]=("VAR");
	listaPalavrasReservadas[18]=("VET");
	
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
			//espaco ou tab
			case 32: case 9:{
				
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
			
			case '"':{
				retornos=gerarTokenString(leitura, i, contadorColuna, contadorLinha);
				i=retornos.i;
				contadorLinha=retornos.contadorLinha;
				contadorColuna=retornos.contadorColuna;
				tokenGerado=retornos.tokenGerado;
				listaTokens.push_back(tokenGerado);
				if(retornos.flagParar){
					flagParar=true;
				}		
			}break;
			
			//constantes e IDs
			default :{
				if((leitura[i]>64 && leitura[i]<91) || (leitura[i]>96 && leitura[i]<123)){
					//cout<<"\n viva?"<<endl;
					retornos=gerarTokenIDR(leitura, i, contadorColuna, contadorLinha, listaPalavrasReservadas);
					i=retornos.i;
					contadorLinha=retornos.contadorLinha;
					contadorColuna=retornos.contadorColuna;
					tokenGerado=retornos.tokenGerado;
					if(retornos.flagParar){
						flagParar=true;
					}
					listaTokens.push_back(tokenGerado);
				}else if((leitura[i]>47 && leitura[i]<58)){
					retornos=gerarTokenNum(leitura, i, contadorColuna, contadorLinha);
					i=retornos.i;
					contadorLinha=retornos.contadorLinha;
					contadorColuna=retornos.contadorColuna;
					tokenGerado=retornos.tokenGerado;
					if(retornos.flagParar){
						flagParar=true;
					}
					listaTokens.push_back(tokenGerado);
				}else{
					imprimirErro(contadorLinha, contadorColuna);
					flagParar=true;
				}
			}break;
		}
	}

	for(int i=0; i<listaTokens.size(); i++){
		cout<<"\n"<<listaTokens[i].tokenName<<" "<<listaTokens[i].tokenValue<<"\n";
	}
	
	if(flagParar){
		return 0;
	}else{
		cout<<"OK\n";
	}
	
	//analise sintatica
	
	return 0;
}
