#!/bin/bash
# Guardar el directorio actual

# Verificar si se proporcionó un argumento
if [ $# -gt 0 ]; then

    # Ejecutar ceedling con el argumento proporcionado
    ceedling test:$1
else
    rm -rf test/build
        # Si no hay argumento, ejecutar ceedling sin argumentos adicionales
    ceedling
fi