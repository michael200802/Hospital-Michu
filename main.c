#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <wchar.h>

//-----------------------------------------------------------------------------------------------------------------------------------

//Matrices y vectores donde se guardan los datos

int maxima_longitud_de_una_entrada = 100;

int maximo_numero_de_pacientes = 100;

int numero_de_datos_del_paciente = 5;

//Posiciones de cada dato
int cedula = 0;
int nombre = 1;
int edad = 2;
int area = 3;
int costo = 4;

//Nombres de las areas a las cual puede ir un paciente
char* nombres_de_las_areas[] = {"Emergencia","Traumatologia","Medicina general","Obstetricia" };

// Aqui se guardaran los datos de cada paciente
char info_del_paciente[100][5][100];
//Si se quiere acceder a un dato se hace lo siguiente
// info_del_paciente[numero de paciente][dato]

//Indica el numero de pacientes
//Tambien indica la posicion en la que van los datos de los nuevos pacientes que se quieran registrar (despues que se complete el registro, se aumenta en uno)
int numero_de_pacientes = 0;

//Indica si se esta registrando un nuevo paciente
//0 --- No
//1 --- Si
//Si registrando_paciente es 1, se indica que se ha ingresado la cedula, el nombre y la edad de un paciente pero falta el area y el costo
int registrando_paciente = 0;

//-----------------------------------------------------------------------------------------------------------------------------------

//buffer auxiliar
//Sirve cuando el buffer principal va a ser modificado dentro de poco y queremos conservar la entrada guardada en el
//Tambien puede servir para los intercambios que se dan en la funcion que se encarga de ordenar a los pacientes
char aux[100];

//Este buffer se encargara de almacenar cualquier entrada
char buffer[100];

//Siempre que se almacene una entrada en el buffer, esta variable será igual a la longitud de la entrada en cuestion
int longitud_del_buffer = 0;

//-----------------------------------------------------------------------------------------------------------------------------------

//validar Entrada
//Estas funciones se encargaran de validar el Entrada
//0 ---> falso
//1 ---> verdadero

int es_entero()
{
    //Si la longitud del buffer es cero, no hay numero alguno ahi
    if(longitud_del_buffer == 0)
    {
        return 0;
    }
    //Se recorre el buffer para ver si todos los elementos son digitos
    int i;
    for(i = 0; i < longitud_del_buffer; i++)
    {
        //Si el caracter no es un digito, se regresa cero
        if(isdigit(buffer[i]) == 0)
        {
            return 0;
        }
    }
    //Si es un entero
    //Se debe aclarar que no se permitiran numeros enteros con signos + o - (+1 o -2 por ejemplo)
    return 1;    
}

int es_cedula()
{
    //Si la longitud de la entrada almacenada en el buffer es diferente de 10 (numero de digitos de una cedula), se regresa cero
    if(longitud_del_buffer != 10)
    {
        return 0;
    }
    //se comprueba si es un entero
    return es_entero();
}

int es_edad()
{
    //Si la longitud de la entrada es mayor o igual a 3 (implicando una edad desmesurada), se regresa cero
    if(longitud_del_buffer >= 3)
    {
        return 0;
    }
    //se comprueba si es un entero
    return es_entero();
}


int es_letra(int letra)
{   
	//Verifica si es una letra basandose en el codigo numerico
    switch (letra)
    {
    case -92: // � 
        break;
    case -91: // �
        break;
    case -96: // a con tilde
        break;
    case -126:// e con tilde
        break;
    case -95: // i con tilde
        break;
    case -94: // o con tilde
        break;
    case -93: // u con tilde
        break;
    default:
        if(isalpha(letra) == 0)
        {
            return 0;
        }
        break;
    }
    return 1;
}

int es_nombre()
{
    int i;
    
    //Se guarda la posicion del primer espacio que se encuentre (si es que hay por lo menos un espacio)
    //Solo se permitira un espacio como minimo y maximo
    int posicion_del_espacio = -1;
    for (i = 0; i < longitud_del_buffer; i++)
    {
        //El elemento es un espacio
        if(buffer[i] == ' ')
        {
            //Se guarda la posicion y se rompe el ciclo for
            posicion_del_espacio = i;
            break;
        }
    }

    //Si posicion_del_espacio sigue teniendo su valor original (-1), no se ha encontrado ningun espacio
    //Si no se ha encontrado un espacio, se regresa cero pues no es algo permisible.
    if(posicion_del_espacio == -1)
    {
        return 0;
    }

    //Si el espacio esta al final o al principio, se regresa cero pues no es algo permisible
    if(posicion_del_espacio == longitud_del_buffer-1 || posicion_del_espacio == 0)
    {
        return 0;
    }

    //Se revisa que todos los caracteres (situados antes del espacio) sean letras
    for(i = 0; i < posicion_del_espacio ; i++)
    {
        //Si el elemento no es una letra, se regresa cero
		if(es_letra(buffer[i]) == 0)
        {
            return 0;
        }  
    }

    //Se revisa que todos los caracteres (situados despues del espacio) sean letras
    for(i = posicion_del_espacio+1; i < longitud_del_buffer; i++)
    {
        //Si el elemento no es una letra, se regresa cero
        if(es_letra(buffer[0]) == 0)
        {
            return 0;
        }  
    }

    //Todos son letras a excepcion del espacio de la mitad
    return 1;
}

int es_costo()
{
    //posicion_del_punto debe guardar la posicion del punto (si existe) o indicar que no existe un punto (por eso es inicializado en cero)
    int posicion_del_punto = 0;
    
    //Se recorre el buffer
    int i;
    for(i = 0; i < longitud_del_buffer; i++)
    {
        //si es un punto, se guarda la posicion y se termina el ciclo for
        if(buffer[i] == '.')
        {
            posicion_del_punto = i;
            break;
        }
        else
        {
            //Si no es un digito, se regresa cero
            if(isdigit(buffer[i]) == 0)
            {
                return 0;
            }
        }
    }

    //Si posicion_del_punto es cero, la entrada no es un numero decimal.
    //Esto se debe a que solo hay dos posibles escenarios si esta variable es cero
    //     1. El punto esta en la primera posicion
    //     2. No hay punto alguno
    if(posicion_del_punto == 0)
    {
        return 0;
    }

    //Mediante esta operacion se puede determinar si hay dos elementos despues del punto
    if(longitud_del_buffer - posicion_del_punto - 1 == 2)
    {
        //Si los dos siguientes elementos son digitos, se regresa 1
        if(isdigit(buffer[posicion_del_punto+1]) != 0 && isdigit(buffer[posicion_del_punto+2]) != 0)
        {
            return 1;
        }
    }

    //los dos elementos despues del punto no son numeros o no hay dos elementos despues de la coma
    //Se regresa cero
    return 0;

}

//Revisa que haya un numero dentro del buffer y que este numero sea parte del intervalo [min;max]
//Se debe aclarar que las opciones no pueden ser numeros negativos
int es_opcion(int min, int max)
{
    //Si no es un entero, se regresa cero
    if(es_entero() == 0)
    {
        return 0;
    }

    //Se obtiene el numero guardado en el buffer y se lo guarda en la variable num
    int num = atoi(buffer);

    //Si num es menor que min o es mayor que max, num no es parte del intervalo [min;max]
    if(num < min || num > max)
    {
        //Se regresa cero pues la entrada no es una valida
        return 0;
    }

    //el numero guardado en el buffer si es una opcion parte del intervalo [min;max]
    return 1;
}

//validar Entrada
//Estas funciones se encargaran de validar el Entrada
//0 ---> falso
//1 ---> verdadero

//-----------------------------------------------------------------------------------------------------------------------------------

//Entrada
//Funciones encargadas del ingreso de datos
//Cada entrada se guarda en el buffer y la longitud de esta entrada se guarda en int longitud_del_buffer

//Esta funcion revisa si la entrada es correcta basandose en el parametro es_correcta
//Su parametro debe ser el valor regresado por una de las funciones encargadas de validar la entrada pues asi se sabe si la entrada es correcta (1) o incorrecta (0)
int es_entrada_correcta(int es_correcta)
{
    //Si la entrada no es correcta (la variable es_correcta es igual a cero), se imprime un mensaje.
    if(es_correcta == 0)
    {
        puts("Entrada incorrecta.");
    }
    //Se regresa el valor de la variable
    return es_correcta;
}

//La función leera la entrada hasta que se ingrese el salto de linea (enter)
void dame_entrada()
{
    scanf("%[^\n]s",buffer);
    //fflush(stdin);
    scanf("\n");
    longitud_del_buffer = strlen(buffer);
}

//Permite que el usuario ingrese entradas hasta que una entrada sea una cedula
void dame_cedula()
{
    //Este ciclo se repite hasta que es_cedula indique que la entrada (guardada en el buffer) es una cedula
    do
    {
        //Se llama a dame_entrada para que el usuario ingrese una entrada
        dame_entrada();
        //Se debe recordar que es_entrada_correcta devueleve el mismo valor pasado como parametro pues su unica funcion es imprimir un mensaje de error cuando la entrada no es valida
    } while (es_entrada_correcta(es_cedula()) == 0);
}

//Permite que el usuario ingrese entradas hasta que una entrada sea una edad
//Regresa la edad ingresada
int dame_edad()
{
    //Este ciclo se repite hasta que es_cedula indique que la entrada (guardada en el buffer) es una edad
    do
    {
        //Se llama a dame_entrada para que el usuario ingrese una entrada
        dame_entrada();
        //Se debe recordar que es_entrada_correcta devueleve el mismo valor pasado como parametro pues su unica funcion es imprimir un mensaje de error cuando la entrada no es valida
    } while (es_entrada_correcta(es_edad()) == 0);

    //Se regresa la edad guardada en el buffer
    return atoi(buffer);
}

//Permite que el usuario ingrese entradas hasta que una entrada sea un nombre
void dame_nombre()
{
    //Este ciclo se repite hasta que es_nombre indique que la entrada (guardada en el buffer) es un nombre (apellido + espacio + nombre)
    do
    {
        //Se llama a dame_entrada para que el usuario ingrese una entrada
        dame_entrada();        
        //Se debe recordar que es_entrada_correcta devueleve el mismo valor pasado como parametro pues su unica funcion es imprimir un mensaje de error cuando la entrada no es valida
    } while (es_entrada_correcta(es_nombre()) == 0);
}

//Permite que el usuario ingrese entradas hasta que una entrada sea un costo
void dame_costo()
{
    //Este ciclo se repite hasta que es_nombre indique que la entrada (guardada en el buffer) es un costo (numero decimal de dos digitos)
    do
    {
        //Se llama a dame_entrada para que el usuario ingrese una entrada
        dame_entrada();        
        //Se debe recordar que es_entrada_correcta devueleve el mismo valor pasado como parametro pues su unica funcion es imprimir un mensaje de error cuando la entrada no es valida
    } while (es_entrada_correcta(es_costo()) == 0);
}

//Permite que el usuario ingrese entradas hasta que una entrada sea un opcion dentro del intervalo [min;max]
//Regresa la opcion ingresada
int dame_opcion(int min, int max)
{
    //Este ciclo se repite hasta que es_nombre indique que la entrada (guardada en el buffer) es un una opcion dentro del intervalo [min;max]
    //Se debe aclarar que las opciones no pueden ser numeros negativos
    do
    {
        //Se llama a dame_entrada para que el usuario ingrese una entrada
        dame_entrada();        
        //Se debe recordar que es_entrada_correcta devueleve el mismo valor pasado como parametro pues su unica funcion es imprimir un mensaje de error cuando la entrada no es valida
    } while (es_entrada_correcta(es_opcion(min,max)) == 0);

    return atoi(buffer);
}

//Entrada
//Funciones encargadas del ingreso de datos
//Cada entrada se guarda en el buffer y la longitud de esta entrada se guarda en int longitud_del_buffer


//-----------------------------------------------------------------------------------------------------------------------------------

//Le pide una cedula a el usuario y luego la busca
//Si se encuentra la cedula, se regresa la posicion de los datos del paciente
//Si no se encuentra la cedula, se regresa -1 para indicar que la cedula nunca fue registrada
int buscar_cedula()
{
    puts("Digite la cedula a buscar: ");
    dame_cedula();

    //Se busca la cedula
    int i;
    for(i = 0; i < numero_de_pacientes; i++)
    {
        //Si son iguales, se regresa la posicion del paciente
        if(strcmp(buffer,info_del_paciente[i][cedula]) == 0)
        {
            return i;
        }
    }

    //No se ha encontrado la cedula
    //Se regresa -1
    return -1;
}

//-----------------------------------------------------------------------------------------------------------------------------------

//Intercambia la posicion de dos pacientes dadas la posicion de cada uno
void intercambiar_pacientes(int pos1, int pos2)
{
    //Para ejecutar esta operacion solo se hace uso de aux (el buffer axiliar) y de strcpy
    //aux guarda los datos del paciente cuya posicion es pos1
    //luego los datos del paciente cuya posicion es pos1 se igualan a los del paciente cuya posicion es pos2
    //luego los datos del paciente cuya posicion es pos2 se igualan a los datos guardados por aux
    //Estas tres operaciones se repiten cada vez por cada dato (nombre, area, cedula, etc...)

    //cedula
    strcpy(aux,info_del_paciente[pos1][cedula]);
    strcpy(info_del_paciente[pos1][cedula],info_del_paciente[pos2][cedula]);
    strcpy(info_del_paciente[pos2][cedula],aux);
    
    //nombre
    strcpy(aux,info_del_paciente[pos1][nombre]);
    strcpy(info_del_paciente[pos1][nombre],info_del_paciente[pos2][nombre]);
    strcpy(info_del_paciente[pos2][nombre],aux);

    //edad
    strcpy(aux,info_del_paciente[pos1][edad]);
    strcpy(info_del_paciente[pos1][edad],info_del_paciente[pos2][edad]);
    strcpy(info_del_paciente[pos2][edad],aux);

    //area
    strcpy(aux,info_del_paciente[pos1][area]);
    strcpy(info_del_paciente[pos1][area],info_del_paciente[pos2][area]);
    strcpy(info_del_paciente[pos2][area],aux);

    //costo
    strcpy(aux,info_del_paciente[pos1][costo]);
    strcpy(info_del_paciente[pos1][costo],info_del_paciente[pos2][costo]);
    strcpy(info_del_paciente[pos2][costo],aux);
}

//Ordena la matriz de manera que:
//Los pacientes menores de 21 años van a la izquiera (al principio), los que tienen una edad igual o mayor a 21 van a la izquiera (al final)
//Se usara el metodo burbuja para ordenar los elementos
void ordenar_pacientes_por_edad()
{
    int i, j;

    //Se ejecuta este ciclo numero_de_pacientes veces
    for(i = 0; i < numero_de_pacientes; i++)
    {
        //Se inicia desde el segundo paciente
        //Se ejecuta este ciclo numero_de_pacientes-1 veces
        //De esta manera siempre se compara el paciente anterior (j-1) con el actual (j) sin ningun problema
        for(j = 1; j < numero_de_pacientes; j++)
        {
            //Se intercambia la posicion entre el paciente actual y el anterior si:
            //El paciente anterior es menor de 21 años
            //El paciente actual tiene una edad mayor o igual a 21 años
            if(atoi(info_del_paciente[j][edad]) >= 21 && atoi(info_del_paciente[j-1][edad]) < 21) 
            {
                intercambiar_pacientes(j,j-1);
            }  
        }
    }
}


//Ordena la matriz de manera que:
//Los pacientes van ordenados de manera alfabetica (tomando en cuenta la primera letra del apellido del paciente)
//Se usara el metodo burbuja para ordenar los elementos
void ordenar_pacientes_por_nombre()
{
    int i, j;

    //Se ejecuta este ciclo numero_de_pacientes veces
    for(i = 0; i < numero_de_pacientes; i++)
    {
        //Se inicia desde el segundo paciente
        //Se ejecuta este ciclo numero_de_pacientes-1 veces
        //De esta manera siempre se compara el paciente anterior (j-1) con el actual (j)
        for(j = 1; j < numero_de_pacientes; j++)
        {
            //Se intercambia la posicion entre el paciente actual y el anterior si:
            //El primer caracter del apellido del paciente actual es menor alfabeticamente que el primer caracter del apellido del paciente anterior
            if(info_del_paciente[j-1][nombre][0] > info_del_paciente[j][nombre][0]) 
            {
                intercambiar_pacientes(j,j-1);
            }  
        }
    }
}


//Ordena la matriz de manera que:
//Los pacientes segun el numero de area de manera ascendente
//Se usara el metodo burbuja para ordenar los elementos
void ordenar_pacientes_por_area()
{
    int i, j;

    //Se ejecuta este ciclo numero_de_pacientes veces
    for(i = 0; i < numero_de_pacientes; i++)
    {
        //Se inicia desde el segundo paciente
        //Se ejecuta este ciclo numero_de_pacientes-1 veces
        //De esta manera siempre se compara el paciente anterior (j-1) con el actual (j)
        for(j = 1; j < numero_de_pacientes; j++)
        {
            //Se intercambia la posicion entre el paciente actual y el anterior si:
            //El numero de area del paciente actual es menor que el numero de area del paciente anterior
            if(atoi(info_del_paciente[j-1][area]) > atoi(info_del_paciente[j][area])) 
            {
                intercambiar_pacientes(j,j-1);
            }  
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------

//Es el espacio que hay entre las columnas (columnas de las tablas que se usan para mostrar la informacion de los pacientes)
char espacio_entre_columnas[] = "             ";

//imprime n espacios
void espacios(int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        printf(" ");
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------

int main()
{	
    freopen("output.txt","w",stdout);
    freopen("input.txt","r",stdin);
    //aqui se guardaran las opciones
    int op = 0;

    //ciclo que se cumple hasta que op sea diferente de 7
    while(op != 7)
    {
        //imprimimos las opciones
        puts("[1]: Registrar datos de paciente.");
        puts("[2]: Registrar area de atencion y costo por paciente.");
        puts("[3]: Rectificar el area donde se encuentra el paciente.");
        puts("[4]: Reporte resumen hospitalario ordenados por edad.");
        puts("[5]: Reporte de pacientes.");
        puts("[6]: Reporte de paciente Ordenado x Area totalizando el costo.");
        puts("[7]: Salir. Finaliza el programa.");

        puts("Digite la opcion");
        op = dame_opcion(1,7);

        //switch con las opciones
        switch (op)
        {
        case 1:
            puts("Cedula:");
            dame_cedula();
            //Se guarda el numero de cedula
            strcpy(info_del_paciente[numero_de_pacientes][cedula],buffer);

            puts("Nombre (apellido + nombre):");
            dame_nombre();
            //Se guarda el nombre
            strcpy(info_del_paciente[numero_de_pacientes][nombre],buffer);
            
            puts("Edad:");
            dame_edad();
            //Se guarda la edad
            strcpy(info_del_paciente[numero_de_pacientes][edad],buffer);

            //Se hace registrando_paciente igual a 1 para indicar que se esta registrando un nuevo paciente
            registrando_paciente = 1;
            break;
        case 2:
            //Si registrando_paciente es cero, no se puede hacer esta operacion
            if(registrando_paciente != 0)
            {
                printf("Paciente: %s\n",info_del_paciente[numero_de_pacientes][nombre]);
                
                puts("Digite el area donde ingresa el paciente: ");
                puts("1 - Emergencia");
                puts("2 - Traumatologia");
                puts("3 - Medicina general");
                puts("4 - Obstetricia");
                dame_opcion(1,4);
                //Se guarda el codigo numerico del area
                strcpy(info_del_paciente[numero_de_pacientes][area],buffer);

                puts("Digite el costo: ");
                dame_costo();
                //Se guarda el costo
                strcpy(info_del_paciente[numero_de_pacientes][costo],buffer);

                numero_de_pacientes++;

                //Ya se ha completado el proceso de registro, registrando_paciente se iguala a cero
                registrando_paciente = 0;
            }
            else
            {
                //Se notifica que no se ha registrado el nombre, edad y numero de cedula del nuevo paciente que se quiere registrar
                puts("No se ha registrado el nombre, edad y numero de cedula del nuevo paciente todavia.");
            }
            break;
        case 3:
            //Si registrando_paciente es cero, no se puede hacer esta operacion
            if(numero_de_pacientes != 0)
            {
                //posicion almacena la posicion del paciente
                int posicion = buscar_cedula();

                //si posicion es -1, no se puede continuar la operacion pues esto indica que no se encontro la cedula
                if(posicion != -1)
                {
                    //Se imprime el nombre del paciente y el area actual
                    printf("Paciente: %s\n",info_del_paciente[posicion][nombre]);
                    printf("Area: %d - %s\n\n",atoi(info_del_paciente[posicion][area]),nombres_de_las_areas[atoi(info_del_paciente[posicion][area])-1]);
                    
                    //Se pide el codigo de la nueva area
                    puts("Ingrese el codigo de la nueva area: ");
                    dame_opcion(1,4);
                    printf("La nueva area es: %s\n\n",nombres_de_las_areas[atoi(buffer)-1]);

                    //Se guarda la entrada del buffer en el buffer axiliar pues se usara el buffer principal dentro de poco
                    strcpy(aux,buffer);

                    puts("En verdad desea efectuar los cambios? SI(1)/NO(2)");
                    if(dame_opcion(1,2) == 1)
                    {
                        strcpy(info_del_paciente[posicion][area],aux);
                        puts("Se ha rectificado el area.");
                    }
                    else
                    {
                        puts("No se efectuan los cambios.");
                    }
                }
                else
                {
                    //Se notifica que no se ha registrado dicha cedula
                    puts("No se ha encontrado dicha cedula.");
                }
            }
            else
            {
                puts("No se ha registrado ningun paciente todavia.");
            }

            break;
        case 4:
            //Si registrando_paciente es cero, no se puede hacer esta operacion
            if(numero_de_pacientes != 0)
            {
                //Se ordenan los datos
                ordenar_pacientes_por_edad();

                //Mayores o iguales a 21
                puts("\nPacientes mayores de edad >= 21:\n");

                //Se imprimen el nombre de cada columna
                printf("Cedula%sNombre%sEdad%sArea del hospital\n",espacio_entre_columnas,espacio_entre_columnas,espacio_entre_columnas);


                //Se imprimen 
                int i;
                for(i = 0; i < numero_de_pacientes && atoi(info_del_paciente[i][edad]) >= 21; i++)
                {
                    printf("%s",info_del_paciente[i][cedula]);
                    espacios(strlen("Cedula") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[i][cedula]));
                    printf("%s",info_del_paciente[i][nombre]);
                    espacios(strlen("Nombre") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[i][nombre]));
                    printf("%s",info_del_paciente[i][edad]);
                    espacios(strlen("Edad") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[i][edad]));
                    printf("%s\n",nombres_de_las_areas[atoi(info_del_paciente[i][area])-1]);
                }

                puts("\n");

                //Menores de 21
                puts("Pacientes menores de edad < 21:\n");

                //Se imprimen el nombre de cada columna
                printf("Cedula%sNombre%sEdad%sArea del hospital\n",espacio_entre_columnas,espacio_entre_columnas,espacio_entre_columnas);

                for(; i < numero_de_pacientes; i++)
                {
                    printf("%s",info_del_paciente[i][cedula]);
                    espacios(strlen("Cedula") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[i][cedula]));
                    printf("%s",info_del_paciente[i][nombre]);
                    espacios(strlen("Nombre") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[i][nombre]));
                    printf("%s",info_del_paciente[i][edad]);
                    espacios(strlen("Edad") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[i][edad]));
                    printf("%s\n",nombres_de_las_areas[atoi(info_del_paciente[i][area])-1]);
                }

                puts("\n\n\n");
            }
            else
            {
                puts("No se ha registrado ningun paciente todavia.");
            }

            break;
        case 5:
            //Si registrando_paciente es cero, no se puede hacer esta operacion
            if(numero_de_pacientes != 0)
            {
                //Se ordenan los datos
                ordenar_pacientes_por_nombre();

                puts("\nPacientes\n");

                //Se imprimen el nombre de cada columna
                printf("Cedula%sNombre%sEdad%sArea del hospital\n",espacio_entre_columnas,espacio_entre_columnas,espacio_entre_columnas);

                int i;
                for(i = 0; i < numero_de_pacientes; i++)
                {
                    printf("%s",info_del_paciente[i][cedula]);
                    espacios(strlen("Cedula") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[i][cedula]));
                    printf("%s",info_del_paciente[i][nombre]);
                    espacios(strlen("Nombre") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[i][nombre]));
                    printf("%s",info_del_paciente[i][edad]);
                    espacios(strlen("Edad") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[i][edad]));
                    printf("%s\n",nombres_de_las_areas[atoi(info_del_paciente[i][area])-1]);
                }

                puts("\n\n\n");

            }
            else
            {
                puts("No se ha registrado ningun paciente todavia.");
            }
            break;
        case 6:
            //Si registrando_paciente es cero, no se puede hacer esta operacion
            if(numero_de_pacientes != 0)
            {
                //Ordenamos a los pacientes por numero de area de menor a mayor
                ordenar_pacientes_por_area();

                //Digite el numero de area que se mostrara
                puts("Digite el numero de area que quiere mostrar: ");
                puts("1 - Emergencia");
                puts("2 - Traumatologia");
                puts("3 - Medicina general");
                puts("4 - Obstetricia");
                puts("t - Todas las areas");
                do
                {
                    //Se consigue la entrada
                    dame_entrada();

                    //Si la entrada es un entero se guarda el numero en op
                    if(es_entero() == 1)
                    {
                        op = atoi(buffer);
                    }
                    else
                    {
                        //Se comprueba si se digito solo una letra pues eso significa que se ingreso una letra (puede ser 't')
                        if(longitud_del_buffer == 1)
                        {
                            //se guarda la primera letra de el buffer
                            op = buffer[0];
                        }
                    }
                } while ((op < 1 || op > 4) && op != 't');
                

                //Cada elemento corresponde a una area
                //El ultimo elemento (4) es el total general
                double total[5];

                //Se lo inicializa en cero para poder sumarle costos
                total[4] = 0;

                //En este caso, i es para referirnos al area en el que se esta y j es para referirnos al paciente o a la posicion del paciente en la matriz
                int i = 0, j = 0;

                //Si no es t, se incializa a i en op
                if(op != 't')
                {
                    //Igualamos i a el numuero de opcion -1
                    i = op-1;

                    //Nos movemos hasta llegar al area correspondiente a i
                    for(j = 0; atoi(info_del_paciente[j][area])-1 != i && j < numero_de_pacientes; j++)
                    {
                        
                    }

                    //Si el elemento apuntado por j tiene una area correspondiente a i, el area no esta vacia
                    if(atoi(info_del_paciente[j][area])-1 == i)
                    {
                        //Imprimimos el area del hospital y los titulos de las columnas
                        //espacios_entre_columnas es una cadena de caracteres que determinara el espacio entre columnas
                        printf("\nArea del hospital: %s\n\n",nombres_de_las_areas[i]);
                        printf("Cedula%sNombre%sEdad%sCosto\n",espacio_entre_columnas,espacio_entre_columnas,espacio_entre_columnas);

                        //Se lo inicializa en cero para poder sumarle costos
                        total[i] = 0;

                        //Nos movemos por el area hasta llegar al tope del numero de pacientes o hasta pasar al siguiente area
                        for(; j < numero_de_pacientes && atoi(info_del_paciente[j][area])-1 == i; j++)
                        {
                            //Imprimimos los datos
                            //espacios se usara para imprimir los espacios correspondientes entre cada columna
                            //Eso si, el numero de espacios a imprimir es: espacio maximo en el cual se puede escribir (hasta llegar a la proxima columna) -  espacio que ya se uso para escribir
                            printf("%s",info_del_paciente[j][cedula]);
                            espacios(strlen("Cedula") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[j][cedula]));
                            printf("%s",info_del_paciente[j][nombre]);
                            espacios(strlen("Nombre") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[j][nombre]));
                            printf("%s",info_del_paciente[j][edad]);
                            espacios(strlen("Edad") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[j][edad]));
                            printf("%s\n",info_del_paciente[j][costo]);

                            //Se suman los costos de ese paciente
                            total[i] += atof(info_del_paciente[j][costo]);
                        }
                        //Se suman los costos del area al total general
                        total[4] += total[i];

                        //Se imprime el nombre del area
                        printf("COSTO del area: %s",nombres_de_las_areas[i]);

                        //Se imprimen los espacios necesarios para llegar a la columna
                        espacios(strlen(espacio_entre_columnas)*3 + strlen("CedulaNombreEdad") - strlen("COSTO del area: ") - strlen(nombres_de_las_areas[i])); 

                        //Se imprime el total de esa area
                        printf("%0.2f\n\n\n",total[i]);
                    }

                }
                else
                {
                    //En este caso, necesitamos que i recorra el intervalo [0;3] pues vamos a mostrar todas las areas (si no estan vacias)
                    for(i = 0; i < 4; i++)
                    {
                        //Si el paciente actual no tiene el numero de area indicada por i, la area indicada por i esta vacia y no vale la pena ejecutar el condicional
                        if(atoi(info_del_paciente[j][area])-1 == i)
                        {
                            //Imprimimos el area del hospital y los titulos de las columnas
                            //espacios_entre_columnas es una cadena de caracteres que determinara el espacio entre columnas
                            printf("\nArea del hospital: %s\n\n",nombres_de_las_areas[i]);
                            printf("Cedula%sNombre%sEdad%sCosto\n",espacio_entre_columnas,espacio_entre_columnas,espacio_entre_columnas);

                            //Se lo inicializa en cero para poder sumarle costos
                            total[i] = 0;

                            //Nos moveremos hasta que llegemos al tope del numero de pacientes o a una nueva area
                            for(; j < numero_de_pacientes && atoi(info_del_paciente[j][area])-1 == i; j++)
                            {
                                //Imprimimos los datos
                                //espacios se usara para imprimir los espacios correspondientes entre cada columna
                                //Eso si, el numero de espacios a imprimir es: espacio maximo en el cual se puede escribir (hasta llegar a la proxima columna) -  espacio que ya se uso para escribir
                                printf("%s",info_del_paciente[j][cedula]);
                                espacios(strlen("Cedula") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[j][cedula]));
                                printf("%s",info_del_paciente[j][nombre]);
                                espacios(strlen("Nombre") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[j][nombre]));
                                printf("%s",info_del_paciente[j][edad]);
                                espacios(strlen("Edad") + strlen(espacio_entre_columnas) - strlen(info_del_paciente[j][edad]));
                                printf("%s\n",info_del_paciente[j][costo]);

                                //Se suman los costos de ese paciente
                                total[i] += atof(info_del_paciente[j][costo]);
                            }

                            //Se suman los costos del area al total general
                            total[4] += total[i];

                            //Se imprime el nombre del area
                            printf("COSTO del area: %s",nombres_de_las_areas[i]);

                            //Se imprimen los espacios necesarios para llegar a la columna
                            espacios(strlen(espacio_entre_columnas)*3 + strlen("CedulaNombreEdad") - strlen("COSTO del area: ") - strlen(nombres_de_las_areas[i])); 

                            //Se imprime el total de esa area
                            printf("%0.2f\n\n\n",total[i]);
                        }
                            
                    }
                    //Se imprime el total general
                    printf("COSTO TOTAL GENERAL");
                    espacios(strlen(espacio_entre_columnas)*3 + strlen("CedulaNombreEdad") - strlen("COSTO TOTAL GENERAL"));
                    printf("%0.2f\n\n\n",total[4]);
                }
            }
            else
            {
                puts("No se ha registrado ningun paciente todavia.");
            }
            break;
        }
    }
    
    return 0;
}
