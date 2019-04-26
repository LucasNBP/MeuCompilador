#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

#define Tam_Maximo 524288

struct Tokens{
	string tokenValue;
	string tokenName;
};

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

valoresRetorno gerarTokenMenor(char leitura[Tam_Maximo], int i, int contadorColuna, int contadorLinha){
	valoresRetorno retornos;
	Tokens tokenGerado;
	string acumularToken="";
	acumularToken+=leitura[i];

	i++;
	contadorColuna++;
	switch(leitura[i])
	{
		case '=':{
			acumularToken+=leitura[i];
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName="<=";
		}break;

		case '>':{
			acumularToken+=leitura[i];
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName="<>";
		}break;

		case '-':{
			acumularToken+=leitura[i];
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName="<-";
		}break;

		default :{
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

valoresRetorno gerarTokenMaior(char leitura[Tam_Maximo], int i, int contadorColuna, int contadorLinha){
	valoresRetorno retornos;
	Tokens tokenGerado;
	string acumularToken="";
	acumularToken+=leitura[i];

	i++;
	contadorColuna++;
	switch(leitura[i])
	{
		case '=':{
			acumularToken+=leitura[i];
			tokenGerado.tokenValue=acumularToken;
			tokenGerado.tokenName=">=";
			break;
		}break;

		default :{
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

int main(){
	ios_base::sync_with_stdio(false);

	char leitura[Tam_Maximo];
	int tamanhoLeitura;
	bool flagParar=false;	
	int contadorColuna=0;
	int contadorLinha=1;
	string acumularToken="";
	Tokens tokenGerado;
	valoresRetorno retornos;

	vector<Tokens> listaTokens;
	
	tamanhoLeitura = fread(leitura, sizeof(char), Tam_Maximo, stdin);
	
	/*for(int i=0; i<tamanhoLeitura; i++){
		cout<<(int)leitura[i]<<"\n";
	}
	cout<<"-------------";*/
	
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

	for(int i=0; i<tamanhoLeitura; i++){
		contadorColuna++;
		switch (leitura[i])
		{
			case 10:{
				contadorLinha++;
				contadorColuna=0;
			}break;

			case '=':{
				tokenGerado.tokenName="=";
				tokenGerado.tokenValue="=";
				listaTokens.push_back(tokenGerado);
			}break;

			case '<':{
				retornos=gerarTokenMenor(leitura, i, contadorColuna, contadorLinha);
				i=retornos.i;
				contadorLinha=retornos.contadorLinha;
				contadorColuna=retornos.contadorColuna;
				tokenGerado=retornos.tokenGerado;
				listaTokens.push_back(tokenGerado);
			}break;

			case '>':{
				retornos=gerarTokenMaior(leitura, i, contadorColuna, contadorLinha);
				i=retornos.i;
				contadorLinha=retornos.contadorLinha;
				contadorColuna=retornos.contadorColuna;
				tokenGerado=retornos.tokenGerado;
				listaTokens.push_back(tokenGerado);
			}

		}
	}

	for(int i=0; i<listaTokens.size(); i++){
		cout<<"\n"<<listaTokens[i].tokenName<<"\n";
	}
	
	
	/*
	string teste = "lucas natanael\nlucas natanael";
	cout<<teste;
	*/
	return 0;
}
