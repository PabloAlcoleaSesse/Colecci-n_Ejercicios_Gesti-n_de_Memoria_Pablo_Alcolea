//
// Created by Pablo Alcolea Sesse on 13/11/24.
//
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
        hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE,    // Utilizar archivo de paginación
            NULL,                    // Seguridad predeterminada
            PAGE_READWRITE,          // Permisos de lectura/escritura
            0,                       // Tamaño máximo (parte alta)
            SIZE,                    // Tamaño máximo (parte baja)
            SHARED_MEM_NAME          // Nombre del objeto de mapeo
        );

        if (hMapFile == NULL) {
            printf("No se pudo crear el objeto de mapeo de archivo (%d).\n", GetLastError());
            return 1;
        }

        shared_memory = (char *) MapViewOfFile(
            hMapFile,            // Manejador del objeto de mapeo
            FILE_MAP_ALL_ACCESS, // Permisos de lectura/escritura
            0,
            0,
            SIZE
        );

        if (shared_memory == NULL) {
            printf("No se pudo mapear la vista del archivo (%d).\n", GetLastError());
            CloseHandle(hMapFile);
            return 1;
        }

        // Escribir en la memoria compartida
        strcpy(shared_memory, "¡Hola, proceso hijo!");

        // Crear el proceso hijo
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // Construir la línea de comandos para el proceso hijo
        char cmdLine[MAX_PATH];
        sprintf(cmdLine, "%s child", argv[0]);

        if (!CreateProcess(
                NULL,           // No hay nombre de módulo (usar línea de comandos)
                cmdLine,        // Línea de comandos
                NULL,           // Seguridad del proceso
                NULL,           // Seguridad del hilo
                FALSE,          // Manejo de herencia de handles
                0,              // Flags de creación
                NULL,           // Bloque de ambiente
                NULL,           // Directorio de trabajo
                &si,            // Información de inicio
                &pi             // Información del proceso
        )) {
            printf("Error al crear el proceso hijo (%d).\n", GetLastError());
            UnmapViewOfFile(shared_memory);
            CloseHandle(hMapFile);
            return 1;
        }

        // Esperar a que el proceso hijo termine
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Cerrar handles del proceso e hilo
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        // Liberar la memoria compartida
        UnmapViewOfFile(shared_memory);
        CloseHandle(hMapFile);
    } else {
        // Proceso hijo
        hMapFile = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,   // Permisos de lectura/escritura
            FALSE,                 // No heredar el nombre
            SHARED_MEM_NAME        // Nombre del objeto de mapeo
        );

        if (hMapFile == NULL) {
            printf("No se pudo abrir el objeto de mapeo de archivo (%d).\n", GetLastError());
            return 1;
        }

        shared_memory = (char *) MapViewOfFile(
            hMapFile,            // Manejador del objeto de mapeo
            FILE_MAP_ALL_ACCESS, // Permisos de lectura/escritura
            0,
            0,
            SIZE
        );

        if (shared_memory == NULL) {
            printf("No se pudo mapear la vista del archivo (%d).\n", GetLastError());
            CloseHandle(hMapFile);
            return 1;
        }

        // Leer de la memoria compartida
        printf("El hijo lee: %s\n", shared_memory);

        // Liberar la memoria compartida
        UnmapViewOfFile(shared_memory);
        CloseHandle(hMapFile);
    }

    return 0;
}