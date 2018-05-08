# vibora

Juego clásico de la víbora programado en lenguaje c, usando librería ncurse.h

# Instalación

1. Instalar Git

https://git-scm.com/download/win

2. Instala Cygwin

Cygwin es un entorno tipo Unix y una interfaz de línea de comandos para Microsoft Windows.

Incluye entre otras utilidades GCC (un conjunto de compiladores creados por el proyecto GNU.)

https://www.cygwin.com/setup-x86_64.exe

Si tu Windows es de 32 bits, usa el siguiente enlace:

https://www.cygwin.com/setup-x86.exe

* Instalar ncurses

<img src="https://www.dropbox.com/s/88xi2y8yjnpgjua/ncurse.PNG?raw=1">

* Instalar make

<img src="https://www.dropbox.com/s/j7rt6aemzauy52i/make.PNG?raw=1">

3. Abrir Cygwin64 Terminal

Y navega hasta la carpeta de USUARIO

> USUARIO es nombre se usuario de la PC.

```
cd /
cd cygdrive
cd c
cd Users
cd USUARIO
```

4. Clonar proyecto 'vibora'

```
git clone https://github.com/realyussel/vibora.git
```

5. Compilar y Ejecutar

```
cd vibora
make run
```

# Ejemplos de NCURSE

En el directorio ncurse encontraras programas de ejemplo usando la librería ncurse.h.

```
cd vibora/ncurse
ls
```

Puedes ejecutar respectivamente cada programa de la siguiente forma:

```
make target=hello
```

Donde 'hello' es el nombre del archivo .c del programa
