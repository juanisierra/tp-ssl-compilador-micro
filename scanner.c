#include<ctype.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#define TAMTABLA 200
typedef enum{
    INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO, PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT
} TOKEN;

typedef struct
{
    TOKEN unToken;
    char contenido[33];
}datoTabla;

int columna(int c)
{
    switch(c)
    {
        case '+' : return 2;
        case '-' : return 3;
        case '(' : return 4;
        case ')' : return 5;
        case ',' : return 6;
        case ';' : return 7;
        case ':' : return 8;
        case '=' : return 9;
        case EOF : return 10;
        default:
          if(isdigit(c)) return 1;
          else if(isalpha(c)) return 0;
          else if(isspace(c)) return 11;
          else return 12;
    }
}

TOKEN esPalabraReservada(char*s)
{
    if(strcmp(s, "inicio")==0) return INICIO;

    else if(strcmp(s, "fin")==0) return FIN;

    else if(strcmp(s, "leer")==0) return LEER;

    else if(strcmp(s, "escribir") == 0) return ESCRIBIR;

    return -1;//DEVUELVE -1 EN CASO DE NO SER PALABRA RESERVADA.
}

TOKEN automata(FILE* archivo, datoTabla* tablita, char* bufferARetornar, int*lineaActual)
{
    char buffer[33];
    static int tabla[15][13] = {{1,3,5,6,7,8,9,10,11,14,13,0,14},
                                {1,1,2,2,2,2,2,2,2,2,2,2,2},
                                {99,99,99,99,99,99,99,99,99,99,99,99,99},
                                {4,3,4,4,4,4,4,4,4,4,4,4,4},
                                {99,99,99,99,99,99,99,99,99,99,99,99,99},
                                {99,99,99,99,99,99,99,99,99,99,99,99,99},
                                {99,99,99,99,99,99,99,99,99,99,99,99,99},
                                {99,99,99,99,99,99,99,99,99,99,99,99,99},
                                {99,99,99,99,99,99,99,99,99,99,99,99,99},
                                {99,99,99,99,99,99,99,99,99,99,99,99,99},
                                {99,99,99,99,99,99,99,99,99,99,99,99,99},
                                {14,14,14,14,14,14,14,14,14,12,14,14,14},
                                {99,99,99,99,99,99,99,99,99,99,99,99,99},
                                {99,99,99,99,99,99,99,99,99,99,99,99,99},
                                {99,99,99,99,99,99,99,99,99,99,99,99,99}};

    int estado_actual = 0;
    int posBuffer = 0;
    int car = fgetc(archivo);
    if(car == '\n') (*lineaActual) += 1;
    estado_actual = tabla[estado_actual][columna(car)];
    while(estado_actual == 0 || estado_actual == 1 || estado_actual == 3 || estado_actual == 11 || estado_actual == 14 || estado_actual == 99) // aca hacemos que no salga del automata a menos que este en un estado final, no hay transiciones de los finales a los otros
    {

        if(estado_actual == 14)
        {
            printf("Error lexico \n");
            return -1;
        }
        if(estado_actual == 1 || estado_actual == 3)
        {
            buffer[posBuffer] = car;
            posBuffer++;
        }
        car = fgetc(archivo);
        estado_actual = tabla[estado_actual][columna(car)];
        if(estado_actual==2 || estado_actual==4)
        {
           ungetc(car, archivo);
        }
    }
    buffer[posBuffer] = '\0';
    strcpy(bufferARetornar, buffer); // Aca hacemos que se pueda retornar el buffer
    //ESTE SWITCH DEVUELVE EL TOKEN CORRESPONDIENTE A LO LEIDO POR EL BUFFER
    //Y EN CASO DE SER UN IDENTIFICADOR O UNA CONSTANTE, LO GUARDA EN LA TABLA
    switch(estado_actual)
    {
        case 2:
        {
            if(esPalabraReservada(buffer) != -1)
            {
                return esPalabraReservada(buffer);//esPalabraReservada DEVUELVE EL TOKEN CORRESPONDIENTE

            }
            else
            {
                if(posBuffer <= 32) // NOS FIJAMOS QUE SE HAYAN LEIDO MENOS DE 32 CARACTERES
                {
                    //ES IDENTIFICADOR VALIDO
                    return ID;
                }
            }
        }
        case 4:
        {
            // ACA DEVOLVERIA UNA CONSTANTE
            return CONSTANTE;
        }
        case 5: return SUMA;

        case 6: return RESTA;

        case 7: return PARENIZQUIERDO;

        case 8: return PARENDERECHO;

        case 9: return COMA;

        case 10: return PUNTOYCOMA;

        case 12: return ASIGNACION;

        case 13: return FDT; // SE DETECTA EL FDT (FIN DE ARCHIVO)
    }
}




