Para compilar, usar el comando: 

'make compile'

o simplemente:

'make'

Para eliminar el archivo compilado, usar: 

make clean

Para ejecutar, usar: 

make run i=1 k=2 c=3 r=20

Donde i puede ser cualquier valor de instancia [1,100]
Donde k puede ser cualquier cantidad de drones [1,8]
Donde c puede ser cualquier capacidad de dron {3,6,10}
Donde r puede ser cualquier cantidad de restarts mayor a 0. (Toda experimentación se hizo con r=20)

El ejecutar el programa, se generará el archivo output.txt esperado, junto con un archivo path.txt.
Para visualizar el archivo path.txt basta con ejecutar el archivo Path_Visualizer.py adjunto.

La carpeta MVDRP_Instances debe estar en la misma carpeta que el ejecutable.
El entorno de desarollo fue Linux, usando WSL en Windows 10.

Tomas Barros, 201873014-9, Entregable 3, MVDRP-k con Hill-Climbing Mejor Mejora.