# vibora

Juego clásico de la víbora programado en lenguaje c, utilizando la librería ncurse.h

# Instalación

1. Instalar __Git__

https://git-scm.com/download/win

2. Instalar __Cygwin__

Cygwin es un entorno tipo Unix y una interfaz de línea de comandos para Microsoft Windows.

Incluye entre otras utilidades GCC (un conjunto de compiladores creados por el proyecto GNU.)

https://www.cygwin.com/setup-x86_64.exe

Si tu Windows es de 32 bits, usa el siguiente enlace:

https://www.cygwin.com/setup-x86.exe

* Instalar __ncurses__

<img src="https://www.dropbox.com/s/88xi2y8yjnpgjua/ncurse.PNG?raw=1">

* Instalar __make__

<img src="https://www.dropbox.com/s/j7rt6aemzauy52i/make.PNG?raw=1">

3. Abrir el programa __Cygwin64 Terminal__

Y navega hasta la carpeta de USUARIO

> USUARIO es el nombre de usuario actual en la computadora.

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

En el directorio /ncurse encontraras programas de ejemplo con la librería ncurse.h.

```
cd vibora/ncurse
ls
```

Puedes compilar y ejecutar respectivamente __cada programa__:

```
make target=hello
```

Donde ´hello´ es el nombre del "archivo .c" de un programa.
