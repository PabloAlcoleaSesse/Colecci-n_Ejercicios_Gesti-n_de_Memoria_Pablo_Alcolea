# Colecci-n_Ejercicios_Gesti-n_de_Memoria_Pablo_Alcolea#include <sys/mman.h>

https://github.com/PabloAlcoleaSesse/Colecci-n_Ejercicios_Gesti-n_de_Memoria_Pablo_Alcolea.git

Explicación paso a paso:

Primero, importamos las bibliotecas necesarias para la asignación de memoria y la creación de procesos.

Creamos una constante SIZE para determinar el tamaño de la región de memoria compartida que queremos asignar.

En la función principal, llamamos a la función mmap() para asignar una región de memoria que pueda ser compartida entre procesos. Los argumentos de esta función son los siguientes:

NULL: Le estamos pidiendo al sistema operativo que elija la dirección de inicio de la región de memoria.
SIZE: Es el tamaño de la región que queremos asignar.
PROT_READ|PROT_WRITE: Estamos indicando que la región de memoria puede ser leída y escrita.
MAP_SHARED|MAP_ANONYMOUS: Estas banderas indican que la región de memoria será compartida entre procesos y que no estará respaldada por ningún archivo.
-1: Este argumento sería el descriptor de archivo si la memoria estuviera respaldada por un archivo, pero como usamos MAP_ANONYMOUS, podemos poner -1.
0: Este sería el desplazamiento si la memoria estuviera respaldada por un archivo. Como no es el caso, ponemos 0.
Comprobamos si la asignación fue exitosa. Si mmap() devuelve MAP_FAILED, significa que hubo un error.

Creamos un nuevo proceso utilizando la función fork(). Esta función devuelve el PID del proceso hijo al proceso padre y 0 al proceso hijo.

Luego, comprobamos el valor devuelto por fork(). Si es menor que 0, hubo un error al crear el nuevo proceso.

Si el valor devuelto es 0, significa que estamos en el proceso hijo. En este caso, leemos y mostramos la memoria compartida y luego liberamos la memoria utilizando la función munmap().

Si el valor devuelto por fork() es mayor que 0, significa que estamos en el proceso padre. En este caso, escribimos en la memoria compartida, esperamos a que el proceso hijo termine y luego liberamos la memoria.

Espero que esta explicación te ayude a entender mejor el código. Si tienes más preguntas, no dudes en hacerlas.

## Solucion:

Las librerias que se utilizan("sys/mman.h") son librerias de Unix por lo que funcionan en dispositivos de MacOS y Linux.
Para que el codigo funcione hay que modificarlo al codigo como el archivo main_windows.c que se encuentra en el ejecutable Ejercicio_para_windows.

Claro, permíteme explicarte qué hace este código y cómo se relaciona con el código anterior y su explicación.

Resumen del contexto:

•	Código original (UNIX): Utiliza funciones específicas de sistemas UNIX como mmap y fork para crear memoria compartida y un proceso hijo que se comunica con el padre a través de esta memoria.

•	Código modificado (Windows): Adapta el código original para que funcione en sistemas Windows, utilizando las API de Windows como CreateFileMapping, MapViewOfFile y CreateProcess para lograr una funcionalidad equivalente.

Explicación del código proporcionado:

#include <windows.h>
#include <stdio.h>
#include <string.h>

#define SIZE 4096
#define SHARED_MEM_NAME "Local\\MySharedMemory"

int main(int argc, char *argv[]) {
HANDLE hMapFile;
char *shared_memory;

    if (argc == 1) {
        // Proceso padre
        // ... (código del padre)
    } else {
        // Proceso hijo
        // ... (código del hijo)
    }

    return 0;
}

El programa está dividido en dos partes principales:
1.	Proceso Padre (cuando argc == 1):
•	Crea una región de memoria compartida.
•	Escribe un mensaje en la memoria compartida.
•	Crea un proceso hijo que leerá el mensaje.
•	Espera a que el proceso hijo termine.
•	Libera los recursos asociados a la memoria compartida.
2.	Proceso Hijo (cuando argc > 1):
•	Abre la región de memoria compartida creada por el padre.
•	Lee el mensaje de la memoria compartida y lo muestra en pantalla.
•	Libera los recursos asociados a la memoria compartida.

Paso a paso del código:
1.	Inclusión de bibliotecas y definiciones:

•	#include <windows.h>: Para las funciones y tipos de datos de la API de Windows.

•	#include <stdio.h>: Para funciones de entrada/salida (printf).

•	#include <string.h>: Para funciones de manipulación de cadenas (strcpy).

•	#define SIZE 4096: Define el tamaño de la memoria compartida (4 KB).

•	#define SHARED_MEM_NAME "Local\\MySharedMemory": Nombre del objeto de memoria compartida.

3. Función main:

•	El programa verifica argc para determinar si es el proceso padre o hijo.

•	argc == 1: Es el proceso padre.

•	argc > 1: Es el proceso hijo.

4. Proceso Padre:

a. Creación de la memoria compartida:

hMapFile = CreateFileMapping(
INVALID_HANDLE_VALUE,
NULL,
PAGE_READWRITE,
0,
SIZE,
SHARED_MEM_NAME
);

	•	INVALID_HANDLE_VALUE: Indica que no se está mapeando un archivo existente, sino el archivo de paginación del sistema.
	
    •	NULL: Seguridad predeterminada.
	
    •	PAGE_READWRITE: Permite lectura y escritura.
	
    •	SIZE: Tamaño de la memoria compartida.
	
    •	SHARED_MEM_NAME: Nombre del objeto de memoria compartida.

b. Verificación de errores:

•	Si CreateFileMapping devuelve NULL, se muestra un mensaje de error y se termina el programa.

c. Mapeo de la memoria compartida en el espacio de direcciones:

shared_memory = (char *) MapViewOfFile(

hMapFile,

FILE_MAP_ALL_ACCESS,

0,

0,

SIZE

);

	•	hMapFile: Manejador del objeto de memoria compartida.

	•	FILE_MAP_ALL_ACCESS: Permite lectura y escritura.

	•	Si MapViewOfFile devuelve NULL, se maneja el error.

d. Escritura en la memoria compartida:


strcpy(shared_memory, "¡Hola, proceso hijo!");

	•	Copia el mensaje en la memoria compartida.

e. Creación del proceso hijo:


STARTUPINFO si;

PROCESS_INFORMATION pi;

ZeroMemory(&si, sizeof(si));

si.cb = sizeof(si);

ZeroMemory(&pi, sizeof(pi));

char cmdLine[MAX_PATH];

sprintf(cmdLine, "%s child", argv[0]);

if (!CreateProcess(

NULL,

cmdLine,

NULL,

NULL,

FALSE,

0,

NULL,

NULL,

&si,

&pi

)) {

// Manejo de errores

}

	•	Inicializa las estructuras STARTUPINFO y PROCESS_INFORMATION.

	•	Construye la línea de comandos para el proceso hijo, pasando "child" como argumento adicional.

	•	CreateProcess crea el proceso hijo ejecutando el mismo programa con el argumento "child".

f. Esperar a que el proceso hijo termine:

WaitForSingleObject(pi.hProcess, INFINITE);

    •	Espera indefinidamente hasta que el proceso hijo finalice.

g. Liberación de recursos:

•	Cierra los handles del proceso e hilo del hijo.

•	Desmapea la vista de la memoria compartida.

•	Cierra el manejador del objeto de memoria compartida.

	4.	Proceso Hijo:

a. Apertura de la memoria compartida:

hMapFile = OpenFileMapping(

FILE_MAP_ALL_ACCESS,

FALSE,

SHARED_MEM_NAME

);

	•	FILE_MAP_ALL_ACCESS: Permite lectura y escritura.

	•	FALSE: No hereda el nombre.

	•	SHARED_MEM_NAME: Nombre del objeto de memoria compartida.

	•	Si OpenFileMapping devuelve NULL, se maneja el error.

b. Mapeo de la memoria compartida en el espacio de direcciones:

shared_memory = (char *) MapViewOfFile(

hMapFile,

FILE_MAP_ALL_ACCESS,

0,

0,

SIZE

);

	•	Similar al proceso padre.

	•	Si MapViewOfFile devuelve NULL, se maneja el error.

c. Lectura de la memoria compartida:

printf("El hijo lee: %s\n", shared_memory);

	•	Muestra el mensaje escrito por el padre.

d. Liberación de recursos:

•	Desmapea la vista de la memoria compartida.

•	Cierra el manejador del objeto de memoria compartida.

Relación con el código y la explicación anterior:

•	Funcionalidad Similar:

•	Ambos códigos demuestran cómo crear y utilizar memoria compartida para la comunicación entre un proceso padre y su proceso hijo.

•	El proceso padre escribe un mensaje en la memoria compartida, y el hijo lo lee y lo muestra en pantalla.

•	Diferencias en la Implementación:

•	Sistemas Operativos:

•	El código original utiliza funciones específicas de UNIX (mmap, fork, wait) que no están disponibles en Windows.

•	El código modificado utiliza las API de Windows para lograr una funcionalidad equivalente.

•	Creación de Procesos:

•	UNIX: Utiliza fork() para crear un proceso hijo que es una copia del padre.

•	Windows: Utiliza CreateProcess para iniciar un nuevo proceso ejecutando el mismo programa.

•	Memoria Compartida:

•	UNIX: Utiliza mmap con MAP_SHARED y MAP_ANONYMOUS para crear una región de memoria compartida.

•	Windows: Utiliza CreateFileMapping y MapViewOfFile para crear y mapear una región de memoria compartida nombrada.

•	Comunicación entre Procesos:

•	Ambos códigos logran la comunicación entre procesos a través de memoria compartida, pero utilizan mecanismos específicos de cada sistema operativo.

•	Manejo de Recursos:

•	En ambos casos, es importante liberar los recursos (memoria compartida y handles) para evitar fugas de memoria.

•	Sincronización:

•	UNIX: El padre utiliza wait(NULL) para esperar al hijo.

•	Windows: El padre utiliza WaitForSingleObject para esperar al hijo.

Conceptos Clave y Lecciones Aprendidas:

•	Portabilidad del Código:

•	Las funciones y mecanismos utilizados para ciertos objetivos (como la creación de procesos y memoria compartida) pueden variar significativamente entre sistemas operativos.

•	Al portar código entre UNIX y Windows, es necesario adaptar las llamadas al sistema y API utilizadas.

•	Comunicación entre Procesos (IPC):

•	La memoria compartida es una forma eficiente de IPC, pero su implementación depende del sistema operativo.

•	Es esencial entender las herramientas disponibles en cada entorno para implementar IPC correctamente.

•	Creación de Procesos:

•	En UNIX, fork() crea un nuevo proceso duplicando el proceso actual.

•	En Windows, CreateProcess inicia un nuevo proceso que puede ser el mismo programa o uno diferente.

•	Memoria Compartida en Windows:

•	CreateFileMapping: Crea un objeto de memoria compartida, que puede estar respaldado por un archivo o por el archivo de paginación del sistema.

•	MapViewOfFile: Mapea la memoria compartida en el espacio de direcciones del proceso.

•	Objetos Nombrados: Permiten que diferentes procesos accedan a los mismos recursos compartidos.

Cómo Ejecutar el Programa en Windows:

2. Compilación:

•	Utiliza un compilador compatible con Windows como MinGW o Visual Studio.

•	Por ejemplo, con MinGW:

gcc -o shared_memory_example shared_memory_example.c


	•	Asegúrate de que el compilador incluya las bibliotecas de Windows.

	2.	Ejecución:

	•	Ejecuta el programa desde la línea de comandos:

shared_memory_example.exe


	•	La salida debería ser:

El hijo lee: ¡Hola, proceso hijo!

Conclusión:

El código proporcionado es una adaptación del código original para que funcione en sistemas Windows. Aunque el objetivo y la lógica general son los mismos (comunicación entre procesos usando memoria compartida), las implementaciones difieren debido a las diferencias en las APIs y mecanismos de los sistemas operativos.

Este ejercicio demuestra la importancia de comprender las especificidades de cada sistema operativo al desarrollar software portátil y cómo los conceptos fundamentales (como la memoria compartida y la comunicación entre procesos) pueden ser implementados de manera diferente según el entorno.

Si tienes más preguntas o necesitas más aclaraciones sobre algún aspecto específico del código o su relación con el original, no dudes en preguntar.