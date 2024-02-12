#!/bin/bash

# Guardar el directorio actual
# Verificar si se proporcionó un argumento
if [ $# -gt 0 ]; then
    # Ejecutar ceedling con el argumento proporcionado
    ceedling gcov:$1 utils:gcov
else
    rm -rf test/build
    # Si no hay argumento, ejecutar ceedling sin argumentos adicionales
    ceedling gcov:all utils:gcov
fi
    xdg-open test/build/artifacts/gcov/GcovCoverageResults.html

# Volver al directorio original
