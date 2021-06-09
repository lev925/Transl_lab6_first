#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>	
#include <process.h>	
#include <string.h>	
#define MaxSym	26	//	максимальное число	нетерминалов
#define MaxTerm 256	//	максимальное число	символов
#define MaxStr 256	//	максимальное число	правил

char left[MaxStr];	// левая часть правила char
char right[MaxStr][70];	// правая часть правила
int kol;// число правил грамматики
short int f[MaxSym][MaxTerm]; // значение first_1
char Vn[MaxSym];	// список нетерминалов

void GetData(const char* FileName)
// ввод правил грамматики (нетерминал - большая латинская буква)
// пустая цепочка представима символом #
{
	char aa;
	FILE* in = fopen(FileName, "r");
	if (in == NULL) { printf("Отсутствует входной файл", ""); exit(1); }
	int i;
	kol = 0;
	while (!feof(in))
	{
		fscanf(in, "%c", &aa);
		if (!feof(in)) left[kol++] = aa;
		fscanf(in, "%c", &aa);
		fscanf(in, "%c", &aa);
		// прочитан знак ->
		i = 0; // длина правой чaсти правила
		while(!feof(in))
		{
			fscanf(in, "%c", &aa);
			if (!feof(in) && (aa != '\n')) right[kol - 1][i++] = aa;
			else break;
		}
	}
	fclose(in);
}

void PrintFirst()
// вывести множество first_1
{
	int i, j;

	for (j = 0; j < MaxSym; j++) if (Vn[j] != '\0')
	{
		printf(" first(%c) =	", Vn[j]);
		for (i = 0; i < MaxTerm; i++)
			if (f[j][i] != '\0') printf(" %c ", (char)i);
		printf(" \n");
	}
}

int Reset(int in, int ip, int m)
// обновление множества first для нетерминала in по правилу ip
// ip - номер правила, in - номер нетерминала
// m - номер первого символа правила
{
	int i, j, flag;
	short ff[MaxTerm]; memset(ff, 0, MaxTerm * sizeof(short));
	// ff - новое пополнение first_1 
	char n;
	n=right[ip][m];
	if ((n <= 'Z') && (n >= 'A'))
		memcpy(&ff, f[(int)n - (int)'A'], MaxTerm);
	// для нетерминала first добавляем к текущему множеству
	else ff[n]=1; // для терминала first - сам этот терминал
	flag = 0;
	for (i = 0; i < MaxTerm; i++)
	{
		char a = f[in][i];
		if ((i == (int)'#') && (ff[i] != 0) && (right[ip][m + 1] != '\0'))
			// не пустой символ или нет продолжения
		flag=Reset(in,ip,m+1) || flag;
		else f[in][i] = f[in][i] | ff[i];
		if (f[in][i] != a) flag = 1;
	}
	return flag;
}

void First(void)
// построить множество first_1
{
	int flag, i, j, k;
	for (i = 0; i < MaxSym; i++)
	{
		Vn[i] = '\0';
		for (j = 0; j < MaxTerm; j++)f[i][j] = '\0';
	}	// массивы очищены
	for (i=0; i<kol; i++)
	{
		j = (int)left[i] - (int)'A';
		Vn[j] = left[i];
	}	// список нетерминалов построен
	flag=1;
	while (flag)
	{

		flag = 0;
		for (i = 0; i < kol; i++)	// i - номер правила
		{
			j = (int)left[i] - (int)'A';	// j - номер нетерминала
			k = Reset(j,i,0);
			flag = flag || k;
		}
	}
}

int main(int argc, char* argv[])
{
	if (argc <= 1) GetData("input.txt");// без параметра - файл по умолчанию 
	else GetData(argv[1]);	// ввести данные
	First();
	PrintFirst();
}
