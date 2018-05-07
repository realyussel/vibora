# vibora

Juego clásico de la víbora programado en lenguaje c, usando librería ncurse.h

# Instalación

1. Instalar Git

https://git-scm.com/download/win

2. Instala Cygwin

https://www.cygwin.com/setup-x86_64.exe

Cygwin es un entorno tipo Unix y una interfaz de línea de comandos para Microsoft Windows.

Incluye entre otras utilidades GCC (un conjunto de compiladores creados por el proyecto GNU.)

> __Instalar ncurses*__

3. Crear carpeta 'proyectos'

```
cd /cygdrive/c/Users/USUARIO
mkdir proyectos
cd proyectos
```

4. Clonar proyecto 'vibora'

```
git clone https://github.com/realyussel/vibora.git
```

5. Compilar y ejecutar

```
cd /vibora
gcc vibora.c -o vibora -lncurses
./vibora.exe
```
