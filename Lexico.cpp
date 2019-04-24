#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

struct Tokens{
	string tokenValue;
	string tokenName;
};

/*
Reserved words:
	$ATEH $BIT $DE $ENQUANTO $ESCREVA $FIM $FUNCAO $INICIO $INTEIRO $LEIA $NULO $PARA
	$PARE $REAL $RECEBA $SE $SENAO $VAR $VET $. $: $; $<- $+ $- $* $/ $% $** $( $) $[ $] $< $>
	$<= $>= $= $<> $" $& $| $!
*/


int main(){
	ios_base::sync_with_stdio(false);

	char leitura[512];
	int tamanhoLeitura;
	string coletaniaErros;
	bool flagParar=false;
	
	tamanhoLeitura = fread(leitura, sizeof(char), 512, stdin);
	
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
	
	if(!flagParar){
		cout<<"OK\n";
	}
	
	
	/*
	string teste = "lucas natanael\nlucas natanael";
	cout<<teste;
	*/
	return 0;
}
