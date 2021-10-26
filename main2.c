#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//**************************************************************************************************************

//Tabla precargada con las areas (se rellenara en el main)
char areas[4][17];

//Pacientes
char pacientes[100][5][50];
//cedula --> 0
//nombre --> 1
//edad --> 2
//area --> 3
//costo --> 4

//Cuenta el numero de pacientes
int contador = 0;

//Permite guardar un dato en la matriz de caracteres
//paiente indica la posicion del paciente en la matriz
//dato indica el dato que se guardara, estos son los posibles valores:
//cedula --> 0
//nombre --> 1
//edad --> 2
//area --> 3
//costo --> 4
void guardar_dato(int paciente,char cadena[],int dato)
{
    //Solo podemos guardar el dato si paciente es menor o igual al contador de pacientes
    //si paciente es igual a contador, se esta escribiendo los datod de un nuevo paciente
    if(paciente <= contador)
    {
        strcpy(pacientes[paciente][dato],cadena);
    }
}

//**************************************************************************************************************

//ENTRADA Y VALIDACION DE ENTRADAS

//Revisa si el caracter es un numero entero 0-9
int esnumero(char caracter)
{
    //Si no es un entero
    if(caracter < '0' || caracter > '9')
    {
        //Se regresa falso
        return 0;
    }
    //Se regresa verdadero
    return 1;
}

//Revisa si la cadena es un entero
int escadena_entero(char cadena[],int longitud)
{
    //Se recorre toda la cadena
    int i;
    for(i = 0; i < longitud; i++)
    {
        //Si no es un entero
        if(cadena[i] < '0' || cadena[i] > '9')
        {
            //Se regresa falso
            return 0;
        }
    }
    //Se regresa verdadero
    return 1;
}

//Revisa si la cadena es un numero con dos decimales
int escadena_costo(char cadena[],int longitud)
{
    int i;

    for(i = 0; i < longitud; i++)
    {
        //Se hemos encontrado el '.' 
        if(cadena[i] == '.')
        {
            //SI el '.' esta al inicio o al final, se regresa falso
            if(i == 0 || i == longitud-1)
            {
                puts("Punto al incio o final");
                return 0;
            }
            //Se rompe el ciclo
            break;
        }
        else
        {
            //Si no es un entero
            if(esnumero(cadena[i]) == 0)
            {
                puts("Hay algo que no es un entero");
                //Se regresa falso
                return 0;
            }
        }
    }

    //Si i es igual a la longitud, se ha recorrido toda la cadena y no se ha encontrado ni un punto
    //Si longitud -i -1 no es igual a 2, no hay exactamente dos decimales
    if(i == longitud || longitud - i - 1 != 2)
    {
        //Se regresa falso
        return 0;
    }

    //Se recorre el resto de la cadena
    for (i += 1; i < longitud; i++)
    {
        //Si el caracter no es un entero
        if(esnumero(cadena[i]) == 0)
        {
            //Se regresa falso
            return 0;
        }
    }    
    //Se regresa verdadero
    return 1;

}

//Revisa si el caracter es una letra
int esletra(char caracter)
{
    if(caracter >= 'a' && caracter <= 'z' || caracter >= 'A' && caracter <= 'Z')
    {
        return 1;
    }
    return 0;
}

//Revisa si la cadena es un Apellido 
int escadena_nombre(char cadena[],int longitud)
{
    //Posicion del espacio que separa al apellido y al nombre
    int espacio = -1, i;
    for(i = 0; i < longitud; i++)
    {
        //Si nos hemos encontrado con el primer espacio
        if(espacio == -1 && cadena[i] == ' ')
        {
            //Se guarda la posicion del espacio
            espacio = i;
        }
        else
        {
            //No es un caracter
            if(esletra(cadena[i]) == 0)
            {
                //Se regresa cero
                return 0;
            }
        }
    }

    //No se encontro un espacio, el espacio esta al principio o esta al final
    if(espacio == -1 || espacio == 0 || espacio == longitud-1)
    {
        //Se regresa falso
        return 0;
    }

    ///Se regresa verdadero
    return 1;

}

//Revisa si una entrada es correcta basandose en el tipo especificado por la variable 'tipo'
//N ---> nombre
//D ---> numero decimal con dos decimales (costo)
//C ---> Cedula
//E ---> edad
//n ---> numero entero cualquiera
//Si tipo no es igual a los caracteres antes mentcionados, cualquier entrada es valida
//PD: Longitud es el parametro que guarda la longitud de la entrada guardada en la cadena, no es la longitud de la cadena pasada como parametro
int correcta(char cadena[], int longitud, char tipo)
{
    //Guarda si lo es o no lo es
    //Por defecto, loes es falso
    //Solo las funciones del switch pueden volver a loes en verdadero
    int loes = 0;
    //Dependiendo del tipo especificado, se llama a una funcion que valida la cadena
    switch (tipo)
    {
    case 'N':
        loes = escadena_nombre(cadena,longitud);
        break;
    case 'D':
        loes = escadena_costo(cadena,longitud);
        break;
    case 'C':
        if(longitud == 10)
        {
            loes = escadena_entero(cadena,longitud);
        }
        break;
    case 'E':
        if(longitud < 3)
        {
            loes = escadena_entero(cadena,longitud);
        }
        break;
    case 'n':
        loes = escadena_entero(cadena,longitud);
        break;
    default:
        loes = 1;
        break;
    }
    //Se regresa loes pues este indica si la cadena es correcta/valida
    return loes;
}

//Pide una entrada hasta que se de una valida
//Cada vez que se pide una entrada, se imprime el mensaje
//Los posibles tipos de entrada son:
//N para un nombre | D para un numero con dos decimales | C para una cedula | 'E' para una edad | 'n' para un numero entero
//PD: Longitud es la longitud de la cadena pasada como parametro o la longitud maxima de la entrada deseada
void entrada(char mensaje[],char cadena[], int longitud, char tipo)
{
    //Ciclo que primero pide una entrada, comprueba si la entrada es del tipo especificado y repite solo si la entrada no era del tipo indicado
    do
    {
        printf(mensaje);
        //Se pida una entrada, la longitud maxima es especificada
        fgets(cadena,longitud,stdin);
        //fgets puede que incluya un salto de linea en la cadena, habra que sobreescribir ese caracter e indicar que ahi queremos que termine la cadena
        if(cadena[strlen(cadena)-1] == '\n')
        {
            cadena[strlen(cadena)-1] = '\0'; 
        }
        //Se eliminan los caracteres no leidos
        fflush(stdin);
    } while (correcta(cadena,strlen(cadena),tipo) == 0);
}

//Permite que se ingrese una opcion que esta incluida en el vector cuya longitud es especificada por el parametro longitud
char opciones(char mensaje[],char vector[], int longitud)
{
    //Aqui se guardaran las opciones
    char cadena[2];
    //i recorrera el vector de opciones
    int i;
    do
    {
        entrada(mensaje,cadena,2,' ');
        for(i = 0; i < longitud; i++)
        {
            if(vector[i] == cadena[0])
            {
                break;
            }
        }
    } while (i == longitud);
    return cadena[0];
}

//*****************************************************************************************************************************

//busca un dato en la matriz
//cedula --> 0
//nombre --> 1
//edad --> 2
//area --> 3
//costo --> 4
int buscar_dato(char dato[], int posicion)
{
    //Recorremos la matriz en busqueda del dato
    int i;
    for(i = 0; i < contador; i++)
    {
        //Comparamos el dato con el dato del paciente de la posicion actual
        if(strcmp(pacientes[i][posicion],dato) == 0)
        {
            //Encontramos el dato y regresamos la posicion del paciente que tiene ese dato
            return i;
        }
    }

    //Se ha recorrido toda la matriz y no se encontro nada
    //se regresa -1 en señal de que el dato no existe
    return -1;
}

//*****************************************************************************************************************************

//Intercambia la posicion de dos pacientes dadas la posicion de cada uno
void intercambio_de_posiciones(int pos1, int pos2)
{
    //Tomara el papel de la variable auxiliar que se muestra en el siguiente algoritmo
    char auxiliar[50];

    //Se intercambian los datos haciendo uso del siguiente alritmo
    /*
    aux = a
    a = b
    b = aux
    */
    //Esto se repite por cada dato

    //Cedulas
    strcpy(auxiliar,pacientes[pos1][0]);
    strcpy(pacientes[pos1][0],pacientes[pos2][0]);
    strcpy(pacientes[pos2][0],auxiliar);
    
    //Nombres
    strcpy(auxiliar,pacientes[pos1][1]);
    strcpy(pacientes[pos1][1],pacientes[pos2][1]);
    strcpy(pacientes[pos2][1],auxiliar);

    //Edadades
    strcpy(auxiliar,pacientes[pos1][2]);
    strcpy(pacientes[pos1][2],pacientes[pos2][2]);
    strcpy(pacientes[pos2][2],auxiliar);

    //Areas
    strcpy(auxiliar,pacientes[pos1][3]);
    strcpy(pacientes[pos1][3],pacientes[pos2][3]);
    strcpy(pacientes[pos2][3],auxiliar);

    //Costos
    strcpy(auxiliar,pacientes[pos1][4]);
    strcpy(pacientes[pos1][4],pacientes[pos2][4]);
    strcpy(pacientes[pos2][4],auxiliar);
}

//Usando el metodo burbuja, se ordenan los pacientes
//criterio indica que criterio se usara
//'N' ---> se ordena a los pacientes alfabeticamente
//'A' ---> se ordena ascendentemente a los pacientes tomando en cuenta el numero de area
void ordenar_pacientes(char criterio)
{
    int i, j;

    //Se ejecuta este ciclo contador veces
    for(i = 0; i < contador; i++)
    {
        //Se inicia desde el segundo paciente
        //Se ejecuta este ciclo contador-1 veces
        //De esta manera siempre se compara el paciente anterior (j-1) con el actual (j)
        for(j = 1; j < contador; j++)
        {
            switch (criterio)
            {
            case 'N':
                //Se intercambia la posicion entre el paciente actual y el anterior si:
                //El primer caracter del apellido del paciente actual es menor alfabeticamente que el primer caracter del apellido del paciente anterior
                if(pacientes[j-1][1][0] > pacientes[j][1][0]) 
                {
                    intercambio_de_posiciones(j,j-1);
                }  
                break;
            case 'A':
                //Se intercambia la posicion entre el paciente actual y el anterior si:
                //El numero de area del paciente actual es menor que el numero de area del paciente anterior
                if(atoi(pacientes[j-1][3]) > atoi(pacientes[j][3])) 
                {
                    intercambio_de_posiciones(j,j-1);
                }
                break;
            default:
                return;
                break;
            }
        }
    }
}

//*****************************************************************************************************************************

//Indica la distancia entre columnas
int distancia_entre_columnas = 13;

//Imprime un numero especifico de espacios
//Sera usada para mostrar los datos
void espacios(int numero)
{
    if(numero < 0)
    {
        return;
    }
    int i;
    for(i = 0; i < numero; i++)
    {
        putchar(' ');
    }
}

//Muestra las columnas o los titulos de estas
//0 ----> no mostrar
//1 ---> mostrar
void mostrar_columnas(int CEDULA, int NOMBRE, int EDAD, int AREA, int COSTO)
{
    if(CEDULA == 1)
    {
        printf("Cedula");
        espacios(distancia_entre_columnas);
    }
    if(NOMBRE == 1)
    {
        printf("Nombre");
        espacios(distancia_entre_columnas);
    }
    if(EDAD == 1)
    {
        printf("Edad");
        espacios(distancia_entre_columnas);
    }
    if(AREA == 1)
    {
        printf("Area del hospital");
        espacios(distancia_entre_columnas);
    }
    if(COSTO == 1)
    {
        printf("Costo");
    }
    putchar('\n');
}

//Muestra los datos de un paciente
//paciente indica la posicion del paciente
//es necesario que se llame a mostrar_columnas antes de llamar a esta funcion
void mostrar_paciente(int paciente, int CEDULA, int NOMBRE, int EDAD, int AREA, int COSTO)
{
    if(CEDULA == 1)
    {
        printf("%s",pacientes[paciente][0]);
        espacios(strlen("Cedula") + distancia_entre_columnas - strlen(pacientes[paciente][0]));
    }
    if(NOMBRE == 1)
    {
        printf("%s",pacientes[paciente][1]);
        espacios(strlen("Nombre") + distancia_entre_columnas - strlen(pacientes[paciente][1]));
    }
    if(EDAD == 1)
    {
        printf("%s",pacientes[paciente][2]);
        espacios(strlen("Edad") + distancia_entre_columnas - strlen(pacientes[paciente][2]));
    }
    if(AREA == 1)
    {
        printf("%s",areas[atoi(pacientes[paciente][3])-1]);
        espacios(strlen("Area del hospital") + distancia_entre_columnas - strlen(pacientes[paciente][3]));   
    }
    if(COSTO == 1)
    {
        printf("%s",pacientes[paciente][4]);
    }
    putchar('\n');
}

//*****************************************************************************************************************************

int main()
{	
    //Se precarga la tabla con las areas
    strcpy(areas[0],"Emergencia");
    strcpy(areas[1],"Traumatologia");
    strcpy(areas[2],"Medicina general");
    strcpy(areas[3],"Obstetricia");

    //Vector de las opciones
    char opcion[] = {'1','2','3','4','5','6','7'};

    //ciclo que se cumple hasta que op sea diferente de 7
    while(1 == 1)
    {
        //Opciones
        puts("1.- Registrar datos de paciente.");
        puts("2.- Registrar area de atencion y costo por paciente.");
        puts("3.- Rectificar el area donde se encuentra el paciente.");
        puts("4.- Reporte resumen hospitalario ordenados por edad.");
        puts("5.- Reporte de pacientes.");
        puts("6.- Reporte de paciente Ordenado x Area totalizando el costo.");
        puts("7.- Salir. Finaliza el programa.");

        //switch con las opciones
        switch (opciones("Digite la opcion: ",opcion,7))
        {
        case '1':
            //Se puede intentar elegir la opcion numero 1 dos veces
            //En ese caso, se devuleve a la normalidad a conatdor
            if(contador < 0)
            {
                contador*=-1;
                contador--;
            }
            if(contador < 100)
            {
                //Cadena donde se guardan los datos
                //Es larga pues los nombres pueden ser muy largos
                char cadena[50];

                //Se piden los respectivos datos y se los guarda en la respectiva posicion

                entrada("Cedula: ",cadena,11,'C');

                if(buscar_dato(cadena,0) != -1)
                {
                    puts("Cedula previamente registrada.");
                }
                else
                {
                    guardar_dato(contador,cadena,0);

                    entrada("Nombre (apellido + nombre): ",cadena,50,'N');
                    guardar_dato(contador,cadena,1);
                    
                    entrada("Edad: ",cadena,3,'E');
                    guardar_dato(contador,cadena,2);

                    //se hace contador negativo para indicar que se esta registrando un nuevo paciente
                    //Tambien se le suma uno para evitar que en los casos en que el contador es cero no se pueda hacer negativo
                    contador++;
                    contador*=-1;
                }
            }
            break;
        case '2':
            //Se revisa que se este registrando un paciente en ese momento
            //Es decir, contador debe ser un numero negativo
            if(contador < 0)
            {

                //Se devuleve a la normalidad al contador
                contador*=-1;
                contador--;

                //Aqui se guadaran las entradas
                char cadena[11];

                //Se imprime el nombre del paciente
                printf("Paciente: %s\n",pacientes[contador][1]);
                
                //Se digita el area
                puts("Areas y sus codigos numericos: ");
                puts("1 - Emergencia");
                puts("2 - Traumatologia");
                puts("3 - Medicina general");
                puts("4 - Obstetricia");
                
                //Mediante esto se consigue el numero de area y se lo guarda en la cadena
                cadena[0] = opciones("Numero de area: ",opcion,4);
                cadena[1] = '\0';

                //Se guarda el numero de area
                guardar_dato(contador,cadena,3);

                //Se digita el costo (numero con dos decimales)
                entrada("Digite el costo: ",cadena,11,'D');
                //Se guarda el costo
                guardar_dato(contador,cadena,4);

                //Se ha registrado un nuevo paciente, se le aumenta en 1 al contador
                contador++;
            }
            break;
        case '3':
            //Se revisa que al menos haya un paciente registrado o no se este registrando uno nuevo
            if(contador > 0)
            {
                //almacena la posicion del paciente al que se le quiere reubicar
                int posicion;
                char cedula[11];
                entrada("Ingrese la cedula a buscar: ",cedula,11,'C');

                posicion = buscar_dato(cedula,0);

                //-1 indica que no se ha encontrado la cedula
                //Solo se puede continuar si se ha encontrado la cedula
                if(posicion != -1)
                {
                    printf("Paciente: %s\n",pacientes[posicion][1]);
                    printf("Area: %s\n",areas[atoi(pacientes[posicion][3])-1]);
                    printf("Numero de area: %d\n\n",atoi(pacientes[posicion][3]));
                    
                    //Se pide el codigo de la nueva area y se lo guarda en nueva area
                    char nueva_area[2];
                    nueva_area[0] = opciones("Areas y sus codigos numericos:\n1 - Emergencia\n2 - Traumatologia\n3 - Medicina general\n4 - Obstetricia\nNumero de la nueva area: ",opcion,4);
                    nueva_area[1] = '\0';
                    printf("La nueva area es: %s\n\n",areas[nueva_area[0] - 48 -1]);

                    //Se pregunta por confirmacion
                    if(opciones("En verdad desea efectuar los cambios? 1 es si y 2 es no\n",opcion,2) == '1')
                    {
                        //Guardamos la nueva area
                        guardar_dato(posicion,nueva_area,3);
                        puts("Se efectuan los cambios.");
                    }
                    else
                    {
                        puts("Se cancelo la operacion.");
                    }
                }
                else
                {
                    //Se notifica que no se ha registrado dicha cedula
                    puts("No se ha encontrado dicha cedula.");
                }
            }

            break;
        case '4':
            //Se revisa que al menos haya un paciente registrado o no se este registrando uno nuevo
            if(contador > 0)
            {
                puts("\nPacientes mayores de >= 21:\n");
                mostrar_columnas(1,1,1,1,0);


                //Se imprimen los datos solo de los mayores o iguales a 21
                int i;
                for(i = 0; i < contador; i++)
                {
                    if(atoi(pacientes[i][2]) >= 21)
                    {
                        mostrar_paciente(i,1,1,1,1,0);
                    }
                }

                //Menores de 21
                puts("\nPacientes menores de edad < 21:\n");
                mostrar_columnas(1,1,1,1,0);

                for(i = 0; i < contador; i++)
                {
                    if (atoi(pacientes[i][2]) < 21)
                    {
                        mostrar_paciente(i,1,1,1,1,0);
                    }
                    
                }

                puts("\n\n\n");
            }
            break;
        case '5':
            //Se revisa que al menos haya un paciente registrado o no se este registrando uno nuevo
            if(contador > 0)
            {
                //Se ordenan los datos
                ordenar_pacientes('N');

                puts("\nPacientes\n");

                //Se imprimen el nombre de cada columna
                mostrar_columnas(1,1,1,1,0);

                int i;
                for(i = 0; i < contador; i++)
                {
                    mostrar_paciente(i,1,1,1,1,0);
                }

                puts("\n\n\n");

            }
            break;
        case '6':
            //Se revisa que al menos haya un paciente registrado o no se este registrando uno nuevo
            if(contador > 0)
            {
                //Ordenamos a los pacientes por numero de area de menor a mayor
                ordenar_pacientes('A');

                //Digite el numero de area que se mostrara
                puts("Digite el numero de area que quiere mostrar: ");
                puts("1 - Emergencia");
                puts("2 - Traumatologia");
                puts("3 - Medicina general");
                puts("4 - Obstetricia");
                puts("t - Todas las areas");
                char area[] = {'1','2','3','4','t'};
                
                int op = opciones("Areas a mostrar: ",area,5);
                
                //Cada elemento corresponde a el costo de un area
                //El elemento del final es el total general
                //Se los inicializa en cero para poder sumarles costos
                double total[] = {0,0,0,0,0};

                //En este caso, i es para referirnos al area en el que se esta y j es para referirnos al paciente o a la posicion del paciente en la matriz
                int i = 0, j = 0;

                //Si es t, se inciara desde el primer paciente pues se recorreran todas las reas
                //Si no es t, se inciara en el area indicada pues solo se recorrera esa area
                if(op != 't')
                {
                    //i indicara el area indicada anteriormente
                    i = op-48-1;

                    //Se recorre la matriz hasta llegar al primer paciente perteneciente a esa area
                    for(; j < contador && atoi(pacientes[j][3])-1 != i; j++)
                    {

                    }

                }

                //i indica en que area estamos
                for(; i < 4; i++)
                {
                    //Revisa si el area esta llena o no
                    if(atoi(pacientes[j][3])-1 == i) 
                    {
                        printf("\nArea del hospital: %s\n\n",areas[i]);
                        mostrar_columnas(1,1,1,0,1);
                        //Nos movemos por el area mostrando los datos de cada paciente
                        for(;atoi(pacientes[j][3])-1 == i && j < contador;j++)
                        {
                            //Se muestran los datos
                            mostrar_paciente(j,1,1,1,0,1);
                            //Se suman los costos de ese paciente
                            total[i] += atof(pacientes[j][4]);
                        }

                        if(op != 't')
                        {
                            break;
                        }

                        //Se suman los costos del area al total general
                        total[4] += total[i];

                        //Se imprime el nombre del area
                        printf("COSTO del area: %s",areas[i]);

                        //Se imprimen los espacios necesarios para llegar a la columna
                        espacios(distancia_entre_columnas*3 + strlen("CedulaNombreEdad") - strlen("COSTO del area: ") - strlen(areas[i])); 

                        //Se imprime el total de esa area
                        printf("%0.2f\n\n\n",total[i]);
                    } 
                    
                }

                //Solo imprimimos el costo general si se ha intentado mostrar todas las areas
                if(op == 't')
                {
                    printf("COSTO TOTAL GENERAL");
                    espacios(distancia_entre_columnas*3 + strlen("CedulaNombreEdad") - strlen("COSTO TOTAL GENERAL"));
                    printf("%0.2f\n\n\n",total[4]);
                }

            }
            break;
        case '7':
            return 0;
            break;
        }
        puts("\n\n");
    }
    
    return 0;
}
