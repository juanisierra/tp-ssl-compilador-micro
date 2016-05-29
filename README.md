#Tp de SSL 2015 - Compilador de lenguaje micro    
##Instrucciones:    
El parser recibe como parametros de ejecucion un archivo .m de codigo micro y un archivo de salida.    
    
##Descripcion del Lenguaje    
-El unico tipo de dato es entero.    
-Todos los identificadores son declarados implicitamente y con una longitud maxima de 32 caracteres.    
-Los identificadores deben comenzar con una letra y estan compuestos de letras y digitos.    
-Las constantes son secuencias de digitos(numeros enteros).    
-Hay dos tipos de sentencias:
  * Asignacion: `ID := Expresion;`    
  Expresion es infija y se construye con identificadores, constantes y los operadores `+` y `-`; los parentesis estan permitidos.    
  * Entrada Salida: `leer(lista de IDs);`    
                  `escribir (lista de expresiones);`    
    
-Cada sentencia termina con un punto y coma. El cuerpo de un programa está delimitado por `inicio` y `fin`.    
-`inicio` , `fin`, `leer` y `escribir` son palabras reservadas y deben escribirse en minuscula.
