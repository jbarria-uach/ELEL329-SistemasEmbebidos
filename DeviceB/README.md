# Software Dispositivo B

## Descripción

Está escrito en python y básicamente responde a comandos enviados por el puerto serial desde el Dispositivo A.

## Instalación

Para instalar este software en Raspberry Pi 3B primero se debe verificar si se cuenta con los requerimientos de software.

En Raspberry Pi abrir un terminal y escribir:

```
sudo apt update
sudo apt install python3-venv python3-pip git
```

Luego descargar el código fuente desde github:
```
git clone https://github.com/jbarria-uach/ELEL329-SistemasEmbebidos.git
```

Crear un entorno virtual de python específico para el proyecto e instalar el proyecto en él con pip:
```
cd ELEL329-SistemasEmbebidos/
python3 -m venv venv
source venv/bin/activate
pip install DeviceB/
```
## Uso
El entorno virtual del proyecto debe estar activado. Si se acaba de instalar ya debe estar activado del paso anterior. Se puede saber porque
en el prompt del terminal dice al principio (venv). De todos modos, para activarlo el comando es (ejecutar desde la carpeta principal del
repositorio:
```
source venv/bin/activate
```
Y para ejecutar el proyecto:
```
dev_b
```

Esto intentará conectarse directamente al puerto /dev/ttyACM0. 
En caso de que el Arduino Mega esté conectado al Raspberry Pi en otro puerto (por ejemplo, si fuese /dev/ttyUSB0)
```bash
dev_b -p /dev/ttyUSB0
```

Si el software logró conectarse al puerto serie en el terminal dirá:
```
Serial port connection made
```
