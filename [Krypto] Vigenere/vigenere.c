 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#define DL 200 // je¿eli chce siê wiêcej znaków w kluczu/tekœcie jawnymtrzeba zmieniæ t¹ liczbê
int i, licznik;
int tablica_ASCII[DL]; 
char znak;	 
char haslo[DL];
char encoded[DL];
   
double czestotliwosc[] = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015,
    0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749,
    0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758,
    0.00978, 0.02360, 0.00150, 0.01974, 0.00074
};
 
 
void szyfr()
{
	for (i=0; i<DL; i++)         
    haslo[i]=42;
FILE *we = fopen("plain.txt","r");
FILE *wy = fopen("crypto.txt","w");
FILE *key =fopen("key.txt","r");
	fscanf(key,"%s",haslo);
int pozycja=0;
for (i=0; i<DL; i++)
   if (haslo[pozycja]!=42)
      {
	tablica_ASCII[i]=haslo[pozycja];      
	pozycja++;
      }
   else
       {
	pozycja=0;                           
	i--;
	tablica_ASCII[i]=haslo[pozycja];
	pozycja++;
      }

for (i=0; i<DL; i++)           
     tablica_ASCII[i]-=65;


licznik=0;

   while((znak=getc(we))!=EOF)
	   {
			znak-=97;
			znak+=tablica_ASCII[licznik];
			if(licznik==DL)
			   licznik=0;
			else
			   licznik++;
			znak%=26;
			znak+=97;
			fprintf(wy,"%c",znak);
		     }      
fclose(wy);
fclose(we);
 printf("Szyfrowanie zostalo zakonczone");

}
void deszyfr()
{
for (i=0; i<DL; i++)         
    haslo[i]=42;
FILE *we= fopen("crypto.txt","r");
FILE *wy= fopen("decrypt.txt","w");
FILE *key =fopen("key.txt","r");
	fscanf(key,"%s",haslo);
int pozycja=0;
for (i=0; i<DL; i++)
   if (haslo[pozycja]!=42)
      {
      	tablica_ASCII[i]=haslo[pozycja];      
	pozycja++;
      }
   else
       {
	pozycja=0;                           
        i--;
      	tablica_ASCII[i]=haslo[pozycja];
	pozycja++;
      }

for (i=0; i<DL; i++)                      
     tablica_ASCII[i]-=65;


licznik=0;

   while((znak=getc(we))!=EOF)
	   {
			znak-=97;
			znak-=tablica_ASCII[licznik];
			if(licznik==DL)
			   licznik=0;
                        else
			   licznik++;
			if (znak<0)
			     znak=26-fabs(znak);
			znak+=97;
			fprintf(wy,"%c",znak);
		     }      
fclose(wy);
fclose(we);
printf("Deszyfrowanie zostalo zakonczone");

}

void przyg(){
FILE *we = fopen("tekst.txt","r");
FILE *wy = fopen("plain.txt","w");
   while((znak=getc(we))!=EOF)
	   {
		if ((znak>=65)&&(znak<=90))
		     {
				 znak+=32;
			fprintf(wy,"%c",znak);
		     }
		 else if ((znak>=97)&&(znak<=122))
		     {
			fprintf(wy,"%c",znak);
		     }
		  else {
			  znak=0;}	  
		}   
fclose(wy);
fclose(we);
 printf("Tekst zostal przygotowany");

}

int best_match(double *a, double *b) {
    double sum = 0, fit, d, best_fit = 1e100;
    int i, rotate, best_rotate = 0;
    for (i = 0; i < 26; i++)
        sum += a[i];
    for (rotate = 0; rotate < 26; rotate++) {
        fit = 0;
        for (i = 0; i < 26; i++) {
            d = a[(i + rotate) % 26] / sum - b[i];
            fit += d * d / b[i];
        }
 
        if (fit < best_fit) {
            best_fit = fit;
            best_rotate = rotate;
        }
    }
 
    return best_rotate;
}
 
double freq_every_nth(int *msg, int len, int interval, char *haslo) {
    double sum, d, ret;
    double out[26], accu[26] = {0};
    int i, j, rot;

    for (j = 0; j < interval; j++) {
        for (i = 0; i < 26; i++)
            out[i] = 0;
        for (i = j; i < len; i += interval)
            out[msg[i]]++;
        haslo[j] = rot = best_match(out, czestotliwosc);
        haslo[j] += 'A';
        for (i = 0; i < 26; i++)
            accu[i] += out[(i + rot) % 26];
    }

    for (i = 0, sum = 0; i < 26; i++)
        sum += accu[i];
 
    for (i = 0, ret = 0; i < 26; i++) {
        d = accu[i] / sum - czestotliwosc[i];
        ret += d * d / czestotliwosc[i];
    }
 
    haslo[interval] = '\0';
    return ret;
}

void kryptoa() {
	for (i=0; i<DL; i++)         
         haslo[i]=42;

	FILE *we= fopen("crypto.txt","r");
	fscanf(we,"%s",encoded);
	fclose(we);
    int txt[strlen(encoded)];
    int len = 0, j;
    double fit, best_fit = 1e100;
 
    for (j = 0; encoded[j] != '\0'; j++)
            txt[len++] = encoded[j] - 'a';
	for (i=0; i<60; i++)
         haslo[i]=42;
    for (j = 1; j < DL; j++) {
        fit = freq_every_nth(txt, len, j, haslo);
        if (fit < best_fit) {
			best_fit = fit;
        printf("key:  %s\n", haslo);
        printf("tekst testowy: ");
        FILE *we= fopen("crypto.txt","r");
int pozycja=0;
for (i=0; i<DL; i++)
   if (haslo[pozycja]!=42)
      {
      	tablica_ASCII[i]=haslo[pozycja];      
	pozycja++;
      }
   else
       {
	pozycja=0;                           
        i--;
      	tablica_ASCII[i]=haslo[pozycja];
	pozycja++;
      }

for (i=0; i<DL; i++)                      
     tablica_ASCII[i]-=65;


licznik=0;

  while((znak=getc(we))!=EOF)
	   {
		if ((znak>=65)&&(znak<=90))
		     {
			znak-=65;
			znak-=tablica_ASCII[licznik];   
			if(licznik==DL)                
			   licznik=0;
                        else
			   licznik++;
			if (znak<0)                     
			     znak=26-fabs(znak);
			znak+=65;
			printf("%c",znak);
		     }
		 else if ((znak>=97)&&(znak<=122))
		     {
			znak-=97;
			znak-=tablica_ASCII[licznik];
			if(licznik==DL)
			   licznik=0;
                        else
			   licznik++;
			if (znak<0)
			     znak=26-fabs(znak);
			znak+=97;
			printf("%c",znak);
		     }
	}     

printf("\n");

}}
		printf("Kryptoanaliza zakonczona");
		fclose(we);
 }

int main(int argc,char **argv){
	switch(argv[1][1]){
		case 'p':
		przyg();
		break;
		case 'e':
		szyfr();
		break;
		case 'd':
		deszyfr();
		break;
		case 'k':
		kryptoa();
		break;
		default:
		printf("zly wybor");
		break;};
return 0;
}
