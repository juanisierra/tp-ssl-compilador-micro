#include<ctype.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "scanner.c"
FILE* archivoDestino;
datoTabla dato;
int lineaActual;
datoTabla lValue;
datoTabla rValue;
char nombreArchivoSalida[50];

datoTabla genInfijo(datoTabla e1, char op, datoTabla e2, datoTabla* miTabla)
{
    datoTabla reg;
    datoTabla temporal;
    temporal.unToken = -5;
    static int numTemp = 1;
    char cadtemp[33] = "Temp&";
    char cadNum[33];
    char cadOP[33];
    if(op == '-') strcpy(cadOP, "Restar");
    if(op == '+') strcpy(cadOP, "Sumar");
    sprintf(cadNum, "%d", numTemp);
    numTemp++;
    strcat(cadtemp, cadNum);
    if(e1.unToken == ID) chequear(e1, miTabla);
    if(e2.unToken == ID) chequear(e2, miTabla);
    strcpy(temporal.contenido,cadtemp);
    strcpy(reg.contenido, cadtemp);
    chequear(temporal,miTabla);
    generar(cadOP, e1.contenido, e2.contenido, cadtemp);
    return reg;
}
void asignar()
{
    generar("Almacena", rValue.contenido, lValue.contenido, "");
}
void terminar()
{
    generar("Detiene","","","");
}
void generar(char* s1, char* s2, char* s3, char* s4)
{
    char cadena[100];
    if(s4[0] == '\0')
        sprintf(cadena, "%s %s, %s\n", s1, s2, s3);
    if(s4[0] != '\0')
        sprintf(cadena, "%s %s, %s, %s\n", s1, s2, s3,s4);
    if(s2[0] == '\0')
        sprintf(cadena, "%s\n", s1);
    fputs(cadena, archivoDestino);
}
void chequear(datoTabla dato, datoTabla* tabla)
{
    int i = 0;
    while(tabla[i].unToken != -1)
    {
        if(strcmp(dato.contenido, tabla[i].contenido) == 0) return;
        i++;
    }
    strcpy(tabla[i].contenido, dato.contenido);
    tabla[i].unToken = dato.unToken;
    tabla[i+1].unToken = -1;
    generar("Declara", dato.contenido, "Entera", "");
}

datoTabla procesarID(datoTabla* tabla)
{
    datoTabla reg;
    chequear(dato, tabla);
    reg.unToken = ID;
    strcpy(reg.contenido, dato.contenido);
    return reg;
}
datoTabla procesarCte()
{
    datoTabla t;
    t.unToken = CONSTANTE;
    strcpy(t.contenido, dato.contenido);
    return t;
}
void leer(datoTabla leido)
{
    generar("Read", leido.contenido, "Entera", "");
}
void escribir(datoTabla valor)
{
    generar("Write", valor.contenido, "Entera", "");
}
TOKEN proximoToken(FILE* archivo, datoTabla* miTabla, TOKEN* tokenActual)
{
    if((*tokenActual) == -1)
    {
        (*tokenActual) = automata(archivo, miTabla, dato.contenido, &lineaActual);
        dato.unToken = (*tokenActual);
        return (*tokenActual);
    }
    else
    {
        return (*tokenActual);
    }
}
void match(TOKEN unToken, FILE* archivo, datoTabla* miTabla, TOKEN* tokenActual)
{
    if((*tokenActual) != -1)
    {
        if(unToken != (*tokenActual))
        {
            errorSintactico(unToken, archivo);
        }
        (*tokenActual) = -1;
    }
    else
    {
        dato.unToken = automata(archivo, miTabla, dato.contenido, &lineaActual);
        if(unToken != dato.unToken)
        {
            errorSintactico(unToken, archivo);
        }
    }

}
void objetivo(FILE* archivo, datoTabla* miTabla, TOKEN* tokenActual)
{
    programa(archivo, miTabla, tokenActual);
    match(FDT, archivo, miTabla, tokenActual);
}
void programa(FILE* archivo, datoTabla* miTabla, TOKEN* tokenActual)
{
    match(INICIO, archivo, miTabla, tokenActual);
    listaSentencias(archivo, miTabla, tokenActual);
    match(FIN,archivo, miTabla, tokenActual);
    terminar();
}
void listaSentencias(FILE* archivo, datoTabla* miTabla, TOKEN* tokenActual)
{
    sentencia(archivo, miTabla, tokenActual);
    while(1)
    {
        switch (proximoToken(archivo, miTabla, tokenActual))
        {
            case ID:
                {
                    sentencia(archivo, miTabla, tokenActual);
                    break;
                }
            case LEER:
                {
                    sentencia(archivo, miTabla, tokenActual);
                    break;
                }
            case ESCRIBIR:
                {
                    sentencia(archivo, miTabla, tokenActual);
                    break;
                }
            default:
                return ;
        }
    }
}
void sentencia(FILE* archivo, datoTabla* miTabla, TOKEN* tokenActual)
{
    TOKEN tok;
    tok = proximoToken(archivo, miTabla, tokenActual);
    switch(tok)
    {
        case ID:
            match(ID, archivo, miTabla, tokenActual);
            lValue = procesarID(miTabla);
            match(ASIGNACION, archivo, miTabla, tokenActual);
            expresion(archivo, miTabla, tokenActual);
            match(PUNTOYCOMA, archivo, miTabla, tokenActual);
            asignar();
            break;
        case LEER:
            match(LEER, archivo, miTabla, tokenActual);
            match(PARENIZQUIERDO, archivo, miTabla, tokenActual);
            listaIdentificadores(archivo, miTabla, tokenActual);
            match(PARENDERECHO, archivo, miTabla, tokenActual);
            match(PUNTOYCOMA, archivo, miTabla, tokenActual);
            break;
        case ESCRIBIR:
            match(ESCRIBIR, archivo, miTabla, tokenActual);
            match(PARENIZQUIERDO, archivo, miTabla, tokenActual);
            listaExpresiones(archivo, miTabla, tokenActual);
            match(PARENDERECHO, archivo, miTabla, tokenActual);
            match(PUNTOYCOMA, archivo, miTabla, tokenActual);
            break;
        default:
            errorSintactico(tok, archivo); break;
    }
}

void listaIdentificadores(FILE* archivo, datoTabla* miTabla, TOKEN* tokenActual)
{
    datoTabla lectura;
    match(ID, archivo, miTabla, tokenActual);
    strcpy(lectura.contenido, dato.contenido);
    lectura.unToken = dato.unToken;
    chequear(lectura, miTabla);
    leer(lectura);
    TOKEN t = proximoToken(archivo, miTabla, tokenActual);
    for(t ; t == COMA; t = proximoToken(archivo, miTabla, tokenActual))
    {
        match(COMA, archivo, miTabla, tokenActual);
        match(ID, archivo, miTabla, tokenActual);
        strcpy(lectura.contenido, dato.contenido);
        lectura.unToken = dato.unToken;
        chequear(lectura, miTabla);
        leer(lectura);
    }
}
void listaExpresiones(FILE* archivo, datoTabla* miTabla, TOKEN* tokenActual)
{
    expresion(archivo, miTabla, tokenActual);
    escribir(rValue);
    TOKEN t;
    t = proximoToken(archivo, miTabla, tokenActual);
    for(t; t == COMA; t = proximoToken(archivo, miTabla, tokenActual))
    {
        match(COMA, archivo, miTabla, tokenActual);
        expresion(archivo, miTabla, tokenActual);
        escribir(rValue);
    }
}
void expresion(FILE* archivo, datoTabla* miTabla, TOKEN* tokenActual)
{
    datoTabla operandoIzq, operandoDer;
    char operacion;
    primaria(archivo, miTabla, tokenActual, &operandoIzq);
    TOKEN t = proximoToken(archivo, miTabla, tokenActual);
    for(t; t == SUMA || t == RESTA; t = proximoToken(archivo, miTabla, tokenActual))
    {
        operadorAditivo(archivo, miTabla, tokenActual, &operacion);
        primaria(archivo, miTabla, tokenActual, &operandoDer);
        operandoIzq = genInfijo(operandoIzq, operacion, operandoDer, miTabla);
    }
    rValue = operandoIzq;
}
void primaria(FILE* archivo, datoTabla* miTabla, TOKEN* tokenActual, datoTabla* operandoIzq)
{
    TOKEN t = proximoToken(archivo, miTabla, tokenActual);
    switch(t)
    {
        case ID:
        {
           match(ID, archivo, miTabla, tokenActual);
           (*operandoIzq) = procesarID(miTabla);
           break;
        }
        case CONSTANTE:
        {
            match(CONSTANTE, archivo, miTabla, tokenActual);
            (*operandoIzq) = procesarCte();
            break;
        }
        case PARENIZQUIERDO:
            {
                match(PARENIZQUIERDO, archivo, miTabla, tokenActual);
                expresion(archivo, miTabla, tokenActual);
                (*operandoIzq) = rValue;
                match(PARENDERECHO, archivo, miTabla, tokenActual);
                break;
            }
        default:
            {
                errorSintactico(t, archivo); break;
            }
    }
}
void operadorAditivo(FILE* archivo, datoTabla* miTabla, TOKEN* tokenActual, char* op)
{
    TOKEN t = proximoToken(archivo, miTabla, tokenActual);
    if (t == SUMA || t == RESTA)
    {
        match(t, archivo, miTabla, tokenActual);
        if(t == SUMA) (*op) = '+';
        if(t == RESTA) (*op) = '-';
    }
    else
    {
        errorSintactico(t, archivo);
    }
}
void errorSintactico(TOKEN unToken, FILE* archivo)
{
    printf("Error sintactico en la linea: %d \n", lineaActual);
    fclose(archivo);
    fclose(archivoDestino);
    remove(nombreArchivoSalida);
    exit(0);
}
int main(int argc, char* argv[])
{   int longitudArchivo;
    if(argc==1)
    {

    printf("Numero Incorrecto de Argumentos.\n Se debe ingresar el nombre del archivo fuente con extension .m  seguido del nombre del archivo de salida.\n");
    }
    if(argc != 3){
        printf("Numero incorrecto de argumentos. \n");
        return -1;
    }
    longitudArchivo=strlen(argv[1]);
    if(argv[1][longitudArchivo-2]!='.' || argv[1] [longitudArchivo-1]!='m')
    {
        printf("El archivo de entrada debe ser de extension .m\n");
        return -1;
    }

    lineaActual = 1;
    TOKEN tokenActual = -1;
    datoTabla miTabla[200];
    miTabla[0].unToken = -1;
    FILE* archivo = fopen(argv[1], "r");
    if(archivo!=NULL)
    {
    archivoDestino = fopen(argv[2], "w");
    strcpy(nombreArchivoSalida,argv[2]);
    objetivo(archivo, miTabla, &tokenActual);
    printf("Compilacion Correcta, se creo el archivo %s \n",argv[2]);
    fclose(archivo);
    fclose(archivoDestino);
    } else {
    	printf("El archivo %s no existe\n",argv[1]);
    }
    return 0;
}
